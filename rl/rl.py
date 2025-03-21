import cv2
import numpy as np
import random
import matplotlib.pyplot as plt
import time
import torch
import torch.nn as nn
import torch.optim as optim
from collections import deque
import sys
import os

bindmodule_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../build/Release/lib'))

sys.path.append(bindmodule_path)

import bindmodule


def resize_frame(frame):
    frame = np.average(frame,axis = 2)
    frame = cv2.resize(frame,(84,84),interpolation = cv2.INTER_NEAREST)
    frame = np.array(frame,dtype = np.uint8)
    return frame


    
class Memory():
    def __init__(self,max_len):
        self.max_len = max_len
        self.frames = deque(maxlen = max_len)
        self.actions = deque(maxlen = max_len)
        self.rewards = deque(maxlen = max_len)
        self.done_flags = deque(maxlen = max_len)

    def add_experience(self,next_frame, next_frames_reward, next_action, next_frame_terminal):
        self.frames.append(next_frame)
        self.actions.append(next_action)
        self.rewards.append(next_frames_reward)
        self.done_flags.append(next_frame_terminal)

    def sample(self, batch_size):
        """
        Sample a batch of experiences randomly from memory.
        Returns:
            - states: np.array of shape (batch_size, 4, 84, 84)
            - actions: List of actions
            - rewards: List of rewards
            - next_states: np.array of shape (batch_size, 4, 84, 84)
            - done_flags: List of done flags
        """
        indices = random.sample(range(4, len(self.frames) - 1), batch_size)

        states = []
        actions = []
        rewards = []
        next_states = []
        dones = []

        for index in indices:
            # Retrieve states and next states by stacking 4 consecutive frames
            state = [self.frames[index - 3], self.frames[index - 2], self.frames[index - 1], self.frames[index]]
            next_state = [self.frames[index - 2], self.frames[index - 1], self.frames[index], self.frames[index + 1]]

            states.append(state)
            next_states.append(next_state)
            actions.append(self.actions[index])
            rewards.append(self.rewards[index])
            dones.append(self.done_flags[index])
        
        states = np.array(states, dtype=np.float32)  / 255.0  # Normalize
        next_states = np.array(next_states, dtype=np.float32) /255.0

        return (
            states,
            actions,
            rewards,
            next_states,
            dones,
        )


def initialize_new_game(Game, agent, level):
    """
    Initialize a new game by resetting the game and storing initial dummy experiences.
    """
    Game.loadConfig(level)
    Game.restart()
    Game.step(0)
    starting_frame = resize_frame(np.array(Game.getLastFrame()))

    dummy_action = 0
    dummy_reward = 0
    dummy_done = False
    for _ in range(3):  # Add 3 dummy frames for initial state so as not to extract frames from the past game
        agent.memory.add_experience(starting_frame, dummy_reward, dummy_action, dummy_done)


def make_Game():
    """
    Create and return the environment.
    """
    game = bindmodule.Game()
    return game



def take_step(Game, agent, score, debug=False, training=True):
    """
    Perform a single step in the game, update the agent's memory, and train if necessary.
    """
    agent.total_timesteps += 1
    if training and agent.total_timesteps % 50000 == 0:
        torch.save(agent.model.state_dict(), 'rl/exp/recent_weights.pth')
        print('\nWeights saved temporarily!')

    # Perform the next action and get the next frame and reward
    prev_health = Game.getHealth()
    Game.step(agent.memory.actions[-1])
    next_frame = np.array(Game.getLastFrame())
    health = Game.getHealth()
    positionY = int(Game.getPositionY()/100)
    positionX = int(Game.getPositionX())//3
    reward = (health - prev_health) - abs(positionX)
    if Game.isWin():
        reward = reward + 500000
    done = Game.isGameOver()

    # Preprocess next frame and construct new state
    unresized_frame = next_frame
    next_frame = resize_frame(next_frame)
    new_state = np.array([agent.memory.frames[-3], agent.memory.frames[-2],
                          agent.memory.frames[-1], next_frame], dtype=np.float32)
    new_state = new_state / 255.0  # Normalize pixel values
    new_state = torch.tensor(new_state).to(agent.device) 
    # Select next action
    next_action = agent.get_action(new_state)

    agent.memory.add_experience(next_frame, reward, next_action, done)

    # If the game is over, return the final score
    if done:
        print("DONE")
        return score + reward, True

    # Render the game if debugging
    if debug:
        cv2.imshow('Game', cv2.cvtColor(unresized_frame, cv2.COLOR_RGBA2BGR))
        cv2.waitKey(1)

    # Train the agent if memory has enough experiences
    if training and len(agent.memory.frames) > agent.starting_mem_len:
        agent.learn(debug)

    return score + reward, False


def play_episode(Game, agent, level, duration=10.0, debug=False, training=True):
    """
    Play an episode and return the final score.
    """
    initialize_new_game(Game, agent, level)
    done = False
    score = 0
    start_time = time.time()
    while not done:
        score, done = take_step(Game, agent, score, debug, training)
        if (time.time() - start_time) > duration:
            print("Stopped episode: surpassing duration limit")
            break

    return score


class Agent():
    
    def __init__(self,possible_actions,starting_mem_len,max_mem_len,starting_epsilon,learn_rate, debug = False):
        self.memory = Memory(max_mem_len)
        self.possible_actions = possible_actions
        self.epsilon = starting_epsilon
        self.epsilon_decay = .9/100000 #initially .9/100000
        self.epsilon_min = .05
        self.gamma = .95
        self.learn_rate = learn_rate
        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = self._build_model().to(self.device)
        self.model_target = self._build_model().to(self.device)
        self.model_target.load_state_dict(self.model.state_dict())
        self.model_target.eval()
        self.optimizer = optim.Adam(self.model.parameters(), lr=self.learn_rate)
        self.loss_fn = nn.HuberLoss()
        self.total_timesteps = 0
        self.starting_mem_len = starting_mem_len
        self.learns = 0


    def _build_model(self):
        """
            Define the neural network for Q-value approximation.
        """
        model = nn.Sequential(
            nn.Conv2d(4, 32, kernel_size=8, stride=4),  # Input shape: [batch, 4, 84, 84]
            nn.ReLU(),
            nn.Conv2d(32, 64, kernel_size=4, stride=2),
            nn.ReLU(),
            nn.Conv2d(64, 64, kernel_size=3, stride=1),
            nn.ReLU(),
            nn.Flatten(),
            nn.Linear(64 * 7 * 7, 512), 
            nn.ReLU(),
            nn.Linear(512, len(self.possible_actions))  # Output: Q-values for each action
        )
        return model


    def get_action(self,state):
            """Explore"""
            if np.random.rand() < self.epsilon:
                return random.sample(self.possible_actions,1)[0]

            """Do Best Action"""
            state_tensor = torch.tensor(state, dtype=torch.float32, device=self.device)
            with torch.no_grad():
                q_values = self.model(state_tensor.unsqueeze(0))  # Add batch dimension
            return self.possible_actions[q_values.argmax().item()]



    def learn(self, debug=False):
        """we want the output[a] to be R_(t+1) + Qmax_(t+1)."""
        """So target for taking action 1 should be [output[0], R_(t+1) + Qmax_(t+1), output[2]]"""

        """First we need 32 random valid indicies"""
        if len(self.memory.frames) < 40:
                return 

        # Sample a random batch of experiences
        states, actions, rewards, next_states, dones = self.memory.sample(32)

        states = torch.tensor(states, dtype=torch.float32, device=self.device)
        next_states = torch.tensor(next_states, dtype=torch.float32, device=self.device)
        actions = torch.tensor(actions, dtype=torch.int64, device=self.device)
        rewards = torch.tensor(rewards, dtype=torch.float32, device=self.device)
        dones = torch.tensor(dones, dtype=torch.float32, device=self.device)

        # Get Q-values for all actions from the current model
        q_values = self.model(states)
        # Select Q-values for the actions taken
        q_values = q_values[range(len(actions)), actions]

        # Get Q-values for next states from the target model
        with torch.no_grad():
            next_q_values = self.model_target(next_states).max(dim=1)[0]  # Max Q-value for next state

        """Calculate target Q-values"""
        target_q_values = rewards + (1 - dones) * self.gamma * next_q_values

        """Train the model"""
        loss = self.loss_fn(q_values, target_q_values) 
        self.optimizer.zero_grad()  # Clear gradients
        loss.backward() 
        self.optimizer.step()
        
        """Decrease epsilon and update how many times our agent has learned"""
        if self.epsilon > self.epsilon_min:
            self.epsilon -= self.epsilon_decay
        self.learns += 1
        
        """Every 10000 learned, copy our model weights to our target model"""
        if self.learns % 10000 == 0:
            self.model_target.load_state_dict(self.model.state_dict())
            print('\nTarget model updated')







def train_agent(agent, selected_level, experiment_name):
    levels = bindmodule.LoadLevels.loadAllLevels("game/levels")
    if not levels:
        print("No levels found!")
        return
    # selected_level = 4
    Game = make_Game()
    last_100_avg = [0]
    scores = deque(maxlen = 100)
    max_score = -100000

    for i in range(301):
        # frames = [] # Saving the frames for the gif
        timesteps = agent.total_timesteps
        timee = time.time()
        level = levels[selected_level]
        score = play_episode(Game, agent, level, duration=75.0, debug = False, training=True) #set debug to true for rendering
        scores.append(score)
        if score > max_score:
            max_score = score

        print('Episode: ' + str(i))
        print('Steps: ' + str(agent.total_timesteps - timesteps))
        print('Duration: ' + str(time.time() - timee))
        print('Score: ' + str(score))
        print('Max Score: ' + str(max_score))
        print('Epsilon: ' + str(agent.epsilon))
        print()
        print()

        if i%100==0 and i!=0:
            last_100_avg.append(sum(scores)/len(scores))
            torch.save(agent.model.state_dict(), f'rl/exp/exp_{experiment_name}.pth')
            torch.save(agent.model_target.state_dict(), f'rl/exp/exp_{experiment_name}_target.pth')
    
    try:
        plt.plot(np.arange(0,i+1,100),last_100_avg)
        plt.show()
    except:
        print("Error plotting info")
    
    


def test_agent(agent, selected_level, experiment_name):
    levels = bindmodule.LoadLevels.loadAllLevels("game/levels")
    if not levels:
        print("No levels found!")
        return
    # selected_level = 4
    agent.model.load_state_dict(torch.load(f"rl/exp/exp_{experiment_name}.pth", weights_only=True))
    agent.model_target.load_state_dict(torch.load(f"rl/exp/exp_{experiment_name}_target.pth", weights_only=True))
    agent.model.eval()
    agent.model_target.eval()
    Game = make_Game()
    level = levels[selected_level]
    
    timesteps = agent.total_timesteps
    timee = time.time()
    score = play_episode(Game, agent, level, duration=75.0, debug = True, training=False) #set debug to true for rendering
    print('Steps: ' + str(agent.total_timesteps - timesteps))
    print('Duration: ' + str(time.time() - timee))
    print('Score: ' + str(score))
        

def continue_train_agent(agent, selected_level, experiment_name, train_round=2):
    levels = bindmodule.LoadLevels.loadAllLevels("game/levels")
    if not levels:
        print("No levels found!")
        return
    # selected_level = 4
    agent.model.load_state_dict(torch.load(f"rl/exp/exp_{experiment_name}.pth", weights_only=True))
    agent.model_target.load_state_dict(torch.load(f"rl/exp/exp_{experiment_name}_target.pth", weights_only=True))
    Game = make_Game()
    last_100_avg = [-700]
    scores = deque(maxlen = 100)
    max_score = -100000

    for i in range(301):
        # frames = [] # Saving the frames for the gif
        timesteps = agent.total_timesteps
        timee = time.time()
        level = levels[selected_level]
        score = play_episode(Game, agent, level, duration=75.0, debug = False, training=True) #set debug to true for rendering
        scores.append(score)
        if score > max_score:
            max_score = score

        print('Episode: ' + str(i))
        print('Steps: ' + str(agent.total_timesteps - timesteps))
        print('Duration: ' + str(time.time() - timee))
        print('Score: ' + str(score))
        print('Max Score: ' + str(max_score))
        print('Epsilon: ' + str(agent.epsilon))
        print()
        print()

        if i%100==0 and i!=0:
            last_100_avg.append(sum(scores)/len(scores))
            torch.save(agent.model.state_dict(), f'rl/exp/exp_{experiment_name}_v{train_round}.pth')
            torch.save(agent.model_target.state_dict(), f'rl/exp/exp_{experiment_name}_v{train_round}_target.pth')
    
    try:
        plt.plot(np.arange(0,i+1,100),last_100_avg)
        plt.show()
    except:
        print("Error plotting info")
    

#get selected level as argument
if len(sys.argv) > 1:
    arg = sys.argv[1]
    if arg.isdigit(): 
        selected_level = int(arg)
    else:
        print("Invalid input. Using default value 0.")
        selected_level = 0
else:
    selected_level = 0

# agent = Agent(possible_actions=[0,1,2],starting_mem_len=1000,max_mem_len=750000,starting_epsilon=1,learn_rate=.0005)
agent_test = Agent(possible_actions=[0,1,2],starting_mem_len=1000,max_mem_len=750000,starting_epsilon=0.05,learn_rate=.0005)
# train_agent(agent, selected_level=0, experiment_name="getPosX_h500_fps40")
test_agent(agent_test, selected_level=selected_level, experiment_name="getPosX_h500_fps40_v2")
# continue_train_agent(agent, selected_level=0, experiment_name="getPosX_h500_fps40", train_round=2)