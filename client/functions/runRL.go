package functions

import (
	"fmt"
	"os/exec"
	"io/ioutil"
	"strconv"
	"strings"
    "path/filepath"
)

func RunRL(level int) {
    if(Token==""){
        fmt.Println("The user is not logged in!");
        return
    }
    if(!ContainsLevel(strconv.Itoa(level))){
		fmt.Println("Level ",level," does not exist")
		return
	}
	//Run the external program
	cmd := exec.Command("python3", "../rl/rl.py",strconv.Itoa(level))
	err := cmd.Run()
	if err != nil {
		fmt.Println("Error running the program:", err)
		return
	}

	// Read the stats from the stats.txt file
    parentDir := filepath.Dir(filepath.Dir("."))

	// Construct the full path to the stats.txt file
	statsFile := filepath.Join(parentDir, "stats.txt")
	
	data, err := ioutil.ReadFile(statsFile)
	if err != nil {
		fmt.Println("Error reading stats.txt:", err)
		return
	}

	// Parse the stats (assuming they are in the format: distance, time, hpfinal, totalScore)
	stats := string(data)
	lines := strings.Split(stats, "\n")
	if len(lines) < 4 {
		fmt.Println("Error: Stats file is not in the expected format")
		return
	}

	distance, err := parseFloat(lines[0])
	if err != nil {
		fmt.Println("Error parsing distance:", err)
		return
	}
	time, err := parseFloat(lines[1])
	if err != nil {
		fmt.Println("Error parsing time:", err)
		return
	}
	hpFinal, err := parseFloat(lines[2])
	if err != nil {
		fmt.Println("Error parsing HP Final:", err)
		return
	}
	totalScore, err := parseFloat(lines[3])
	if err != nil {
		fmt.Println("Error parsing Total Score:", err)
		return
	}
    fmt.Println("RL algorithm's score for Level:", int(level))
	fmt.Println("-------------------------------------")
	fmt.Printf("Distance: %.2f\n", distance)
	fmt.Printf("Time: %.2f\n", time)
	fmt.Printf("HP Final: %.2f\n", hpFinal)
	fmt.Printf("Total Score: %.2f\n", totalScore)
}

