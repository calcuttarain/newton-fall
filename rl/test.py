"""
Newton's Fall Python Bindings (Compact)

Usage:
------
1) from bindmodule import Game, LoadLevels
2) Create a Game instance: game = Game(instantiate=False)
3) levels = LoadLevels.loadAllLevels("game/levels")
4) game.loadConfig(levels[0])
5) game.step(action=0|1|2), game.isGameOver(), game.isWin()
6) Inspect frames: frame = game.getLastFrame()
7) Check health: health = game.getHealth()  # returns current health (0-200)

Minimal Random Loop:
--------------------
while not game.isGameOver():
    game.step(random.choice([0,1,2]))
    print(f"Health: {game.getHealth()}")  # monitor health during gameplay

Advanced:
---------
- run() for interactive window
- restart() to reload current level
- Check win condition with health:
    if game.isWin():
        print(f"Won with {game.getHealth()} health remaining!")
"""

import sys
import os

bindmodule_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../build/Release/lib'))
sys.path.append(bindmodule_path)

import math
import bindmodule
import cv2
import numpy as np
import time
import random



def play_game(game, duration=100.0):
    start_time = time.time()
    while not game.isGameOver() and (time.time() - start_time) < duration:
        # Simulăm un comportament natural de control
        action = 0  # 0=nothing, 1=left, 2=right
        game.step(action)
        
    
        if game.isWin():
            print("Level Won!")
        if game.isGameOver():
            print("Game")

def play_game_with_strategy(game):
    # Prima fază: 5 secunde doar dreapta
    start_time = time.time()
    while not game.isGameOver() and (time.time() - start_time) < 5.0:
        game.step(2)  # Constant dreapta
        frame = game.getLastFrame()
        frame = np.array(frame)
        cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
        
        if game.isWin():
            print("Level Won during right movement phase!")
            break
            
        if cv2.waitKey(16) & 0xFF == ord('q'):
            return

    # A doua fază: mișcare aleatoare până la final
    while not game.isGameOver():
        action = random.choice([0, 1, 2])
        game.step(action)
        
        frame = game.getLastFrame()
        frame = np.array(frame)
        cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
        
        if game.isWin():
            print("Level Won during random movement phase!")
            break
            
        if cv2.waitKey(16) & 0xFF == ord('q'):
            return

def play_sinusoidal_movement(game):
    """
    Controls the square using a sinusoidal pattern for movement.
    Oscillates between left and right based on time.
    """
    start_time = time.time()
    period = 2.0 
    
    while not game.isGameOver():
        current_time = time.time() - start_time
        
        sin_value = math.sin(2 * math.pi * current_time / period)
        
        if sin_value > 0.3:
            action = 2  # dreapta
        elif sin_value < -0.3:
            action = 1  # stânga
        else:
            action = 0  # no movement
            
        game.step(action)
        
        # Afișare
        frame = game.getLastFrame()
        frame = np.array(frame)
        cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
        
        if game.isWin():
            print("Victory! Sinusoidal movement successful!")
            break
            
        if cv2.waitKey(16) & 0xFF == ord('q'):
            return False
            
    return game.isWin()

def test_health_after_gameplay(game, duration=3.0):
    """
    Test function that plays randomly for 3 seconds then checks health.
    Shows health monitoring during gameplay.
    """
    start_time = time.time()
    initial_health = game.getHealth()
    print(f"Starting health: {initial_health}")
    
    while not game.isGameOver() and (time.time() - start_time) < duration:
        action = random.choice([0, 1, 2])
        game.step(action)
        
        frame = game.getLastFrame()
        frame = np.array(frame)
        cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
        
        if cv2.waitKey(16) & 0xFF == ord('q'):
            break
    
    final_health = game.getHealth()
    damage_taken = initial_health - final_health
    print(f"\nAfter {duration} seconds:")
    print(f"Final health: {final_health}")
    print(f"Damage taken: {damage_taken}")
    print(f"Survival rate: {(final_health/initial_health)*100:.1f}%")

def main():
    try:
        levels = bindmodule.LoadLevels.loadAllLevels("game/levels")
        if not levels:
            print("No levels found!")
            return

        game = bindmodule.Game()
        selected_level = 0
        
        print(f"\nTesting health monitoring on level {selected_level}")
        game.loadConfig(levels[selected_level])
        test_health_after_gameplay(game)
                
        cv2.destroyAllWindows()

    except Exception as e:
        print(f"Error: {e}")
        return

if __name__ == "__main__":
    main()

