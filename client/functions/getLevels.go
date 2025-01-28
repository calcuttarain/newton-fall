package functions

import (
	"fmt"
	"os"
	"path/filepath"
    "strings"
)

func GetLevels() {
    if(Token==""){
        fmt.Println("The user is not logged in!");
        return
    }
	// Define the directory path where the levels are stored
	dirPath := "../game/levels"

	// Open the directory
	files, err := os.ReadDir(dirPath)
	if err != nil {
		fmt.Println("Error reading directory:", err)
		return
	}

	// Iterate through the files and print their names
	var levelNames []string
	for _, file := range files {
		if !file.IsDir() && filepath.Ext(file.Name()) == ".json" {
			// Remove the ".json" extension
			levelName := strings.TrimSuffix(file.Name(), ".json")
			levelNames = append(levelNames, levelName)
		}
	}

	// Print all the level names in a line
	fmt.Println("Available levels:", levelNames)
}
