import sys
import os

bindmodule_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../build/Release/lib'))

sys.path.append(bindmodule_path)

import bindmodule


import bindmodule
import cv2
import numpy as np
import time

def main():
    # Load levels
    levels = bindmodule.LoadLevels.loadAllLevels("game/levels")
    if not levels:
        print("No levels found!")
        return

    # Initialize game with first level
    game = bindmodule.Game()
    game.loadConfig(levels[0])

    # Main game loop
    while not game.isGameOver():
        # Always move right (action = 2)
        game.step(2)
        
        # Get and display frame
        frame = game.getLastFrame()
        frame = np.array(frame)
        cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
        
        # Wait for 1/60 second
        time.sleep(1/60)
        
        # Press 'q' to quit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

