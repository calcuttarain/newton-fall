# Features

## Feature 1

- **Input:** Game Object Information – Position, speed, and rotation angle of the object; integrity of the object (damage state).
- **Activation:** Physics Engine Execution – Simulates the interaction of the object with obstacles, including collision detection.
- **Action:** Generating Object Movement – Controls are applied to move and orient the object, while avoiding damage from obstacles.
- **Output:** Game State Visualization – Displays the real-time position and condition of the object and the nearby obstacles.

## Feature 2

- **Input:** Player Interaction – Keyboard inputs that control the object’s movement.
- **Activation:** Capture Player Inputs – Processes the keyboard inputs and maps them to the game mechanics.
- **Action:** Update Object Position – Adjusts the object’s trajectory based on player input to navigate through obstacles.
- **Output:** Visual Feedback for Player – Shows the object’s real-time position, remaining integrity, and any immediate impacts on the screen.

## Feature 3

- **Input:** Training Data for RL Agent – Collision and movement data to improve the RL agent’s performance.
- **Activation:** RL Agent Training – Feeds the data into the reinforcement learning model for training and optimization.
- **Action:** Update RL Agent’s Strategy – The agent adjusts its gameplay approach based on learned patterns and feedback.
- **Output:** RL Agent Performance Data – Displays statistics like success rate, number of avoided collisions, and improvement over time.