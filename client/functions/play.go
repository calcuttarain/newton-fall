package functions

import (
	"client/connect"
	"fmt"
	//"os/exec"
	"io/ioutil"
	"strconv"
	"strings"
    "path/filepath"
)

func Play(level int) {
    if(Token==""){
        fmt.Println("The user is not logged in!");
        return
    }
	Run the external program
	cmd := exec.Command("../build/Release/bin/newtons_fall", strconv.Itoa(level))
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

	// Define the mutation query
	query := `
	mutation createScore($score: ScoreInput!) {
		createScore(score: $score) {
			id
			user {
				id
				name
			}
			level
			distance
			time
			hpFinal
			totalScore
		}
	}`

	variables := map[string]interface{}{
		"score": map[string]interface{}{
			"level":     level,
			"distance":  distance,
			"time":      time,
			"hpFinal":   hpFinal,
			"totalScore": totalScore,
		},
	}

	// Send the GraphQL mutation request
	_, err := connect.GraphQLRequest(query, variables, Token)
	if err != nil {
		fmt.Println("Error sending GraphQL mutation:", err)
		return
	}

	// Handle the response (optional: print the result)
	fmt.Println("Score created successfully")
}

// Helper function to parse float values
func parseFloat(value string) (float64, error) {
	value = strings.TrimSpace(value)
	return strconv.ParseFloat(value, 64)
}
