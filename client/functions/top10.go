package functions

import (
	"client/connect"
	"fmt"
)

func Top10(level int) {
    if(Token==""){
        fmt.Println("The user is not logged in!");
        return
    }
	query := `
	query top10Scores($level: Int!) {
		top10Scores(level: $level) {
			user {
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
		"level": level,
	}

	// Send the GraphQL request
	response, err := connect.GraphQLRequest(query, variables, Token)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	// Parse the response
	data, ok := response["data"].(map[string]interface{})
	if !ok {
		fmt.Println("Error: Unable to parse response data")
		return
	}

	// Extract top10Scores data
	top10ScoresData, ok := data["top10Scores"].([]interface{})
	if !ok {
		fmt.Println("Error: Unable to parse top10Scores data")
		return
	}

	// Print the results in a table format
	fmt.Printf("%-5s %-30s %-10s %-10s %-10s %-10s\n", "Rank", "User", "Distance", "Time", "HP Final", "Total Score")
	fmt.Println("------------------------------------------------------------")

	for rank, item := range top10ScoresData {
		scoreData := item.(map[string]interface{})
		userData := scoreData["user"].(map[string]interface{})
		userName := userData["name"].(string)
		distance := scoreData["distance"].(float64)
		time := scoreData["time"].(float64)
		hpFinal := scoreData["hpFinal"].(float64)
		totalScore := scoreData["totalScore"].(float64)

		// Print each player's score in the table
		fmt.Printf("%-5d %-30s %-10.2f %-10.2f %-10.2f %-10.2f\n", rank+1, userName, distance, time, hpFinal, totalScore)
	}
}
