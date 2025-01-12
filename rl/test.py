import sys
import os

bindmodule_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../build/Release/lib'))

sys.path.append(bindmodule_path)


import bindmodule
import cv2
import numpy as np
import time



def main():
    try:
        levels = bindmodule.LoadLevels.loadAllLevels("game/levels")
        if not levels:
            print("No levels found!")
            return

        # Create game in non-headless mode for visualization
        game = bindmodule.RLGame()
        game.loadConfig(levels[0])

        # Loop-ul principal
        while not game.isGameOver():
            # Mișcă la dreapta (action = 2)
            game.step(0)

            
            # Obține și afișează frame-ul
            frame = game.getLastFrame()
            frame = np.array(frame)

            cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
            
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
    
        cv2.destroyAllWindows()

        #     cv2.imshow('Game', cv2.cvtColor(frame, cv2.COLOR_RGBA2BGR))
            
        #     if cv2.waitKey(1) & 0xFF == ord('q'):
        #         break
    
        # cv2.destroyAllWindows()
      
    except Exception as e:
        print(f"Error: {e}")
        return

if __name__ == "__main__":
    main()

