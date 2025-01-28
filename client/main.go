package main

import (
	"fmt"
	"client/connect"
	"client/functions"
	"bufio"
	"os"
	"strconv"
	"strings"
)



// Example usage
func main() {
	// Load .env variables
	if err := connect.LoadEnv(); err != nil {
		fmt.Println("Error loading .env file:", err)
		return
	}
	reader := bufio.NewReader(os.Stdin)

	fmt.Println("Welcome to the CLI! Type 'help' for available commands or 'exit' to quit.")

	for {
		// Prompt for input
		fmt.Print("> ")
		input, err := reader.ReadString('\n')
		if err != nil {
			fmt.Println("Error reading input:", err)
			continue
		}

		// Trim newline and whitespace
		input = strings.TrimSpace(input)

		// Exit the loop if the user types "exit"
		if input == "exit" {
			fmt.Println("Exiting CLI. Goodbye!")
			break
		}

		// Parse the input command
		parts := strings.Split(input, " ")
		command := parts[0]
		args := parts[1:]

		// Handle commands
		switch command {
		case "play":
			if len(args) != 1 {
				fmt.Println("Usage: play <level>")
				continue
			}
			level, _ := strconv.Atoi(args[0])
			// distance, _ := strconv.ParseFloat(args[1], 64)
			// time, _ := strconv.Atoi(args[2])
			// hpfinal, _ := strconv.Atoi(args[3])
			// totalScore, _ := strconv.Atoi(args[4])
			functions.Play(level)

		case "register":
			if len(args) != 2 {
				fmt.Println("Usage: register <name> <password>")
				continue
			}
			functions.Register(args[0], args[1])

		case "deleteAccount":
			functions.DeleteUser()

		case "login":
			if len(args) != 2 {
				fmt.Println("Usage: login <name> <password>")
				continue
			}
			functions.Login(args[0], args[1])

		case "highscore":
			if len(args) != 1 {
				fmt.Println("Usage: highscore <level>")
				continue
			}
			level, _ := strconv.Atoi(args[0])
			functions.Highscore(level)

		case "top10":
			if len(args) != 1 {
				fmt.Println("Usage: top10 <level>")
				continue
			}
			level, _ := strconv.Atoi(args[0])
			functions.Top10(level)

		case "getLevels":
			functions.GetLevels()

		case "help":
			fmt.Println("Available commands:")
			fmt.Println("  addScore <level> <distance> <time> <hpfinal> <totalScore>")
			fmt.Println("  register <name> <password>")
			fmt.Println("  deleteAccount")
			fmt.Println("  login <name> <password>")
			fmt.Println("  highscore <level>")
			fmt.Println("  top10 <level>")
			fmt.Println("  getLevels")
			fmt.Println("  exit")

		default:
			fmt.Println("Unknown command. Type 'help' for a list of commands.")
		}
	}
}
