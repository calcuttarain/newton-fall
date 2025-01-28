package functions

import (
	"client/connect"
	"fmt"
)

func Highscore(level int) {
    if(Token==""){
        fmt.Println("The user is not logged in!");
        return
    }
	query := `
	query highscore($level: Int!) {
		highScore(level: $level) {
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

	// Extract highScore data
	highScoreData, ok := data["highScore"].(map[string]interface{})
	if !ok {
		fmt.Println("Error: Unable to parse highScore data")
		return
	}

	// Extract each field
	levelData, _ := highScoreData["level"].(float64)
	distance, _ := highScoreData["distance"].(float64)
	time, _ := highScoreData["time"].(float64)
	hpFinal, _ := highScoreData["hpFinal"].(float64)
	totalScore, _ := highScoreData["totalScore"].(float64)

	// Print the results in a nice format
	fmt.Println("Highscore for Level:", int(levelData))
	fmt.Println("-------------------------------------")
	fmt.Printf("Distance: %.2f\n", distance)
	fmt.Printf("Time: %.2f\n", time)
	fmt.Printf("HP Final: %.2f\n", hpFinal)
	fmt.Printf("Total Score: %.2f\n", totalScore)
}
