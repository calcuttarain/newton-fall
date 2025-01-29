package connect

import (
	"bufio"
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"net/http"
	"os"
	"strings"
)
// GraphQLRequest sends a GraphQL query to the server and returns the parsed JSON response.
// If the token is not empty, it adds it to the Authorization header.
func GraphQLRequest(query string, variables map[string]interface{}, token string) (map[string]interface{}, error) {
	// Load the .env variable
	backend := os.Getenv("BACKEND")
	if backend == "" {
		return nil, errors.New("BACKEND environment variable is not set")
	}

	// Form the endpoint
	endpoint := backend + "/graphql"

	// Create the payload
	payload := map[string]interface{}{
		"query":     query,
		"variables": variables,
	}

	// Serialize the payload to JSON
	jsonData, err := json.Marshal(payload)
	if err != nil {
		return nil, fmt.Errorf("failed to marshal payload: %v", err)
	}

	// Create the HTTP request
	req, err := http.NewRequest("POST", endpoint, bytes.NewBuffer(jsonData))
	if err != nil {
		return nil, fmt.Errorf("failed to create request: %v", err)
	}

	// Add headers
	req.Header.Set("Content-Type", "application/json")
	if token != "" {
		req.Header.Set("Authorization", "Bearer "+token)
	}

	// Send the request
	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf("failed to execute request: %v", err)
	}
	defer resp.Body.Close()

	// Check for non-200 status codes
	if resp.StatusCode != http.StatusOK {
		return nil, fmt.Errorf("received non-200 status code: %d", resp.StatusCode)
	}

	// Read the response body
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("failed to read response body: %v", err)
	}

	// Parse the response JSON
	var result map[string]interface{}
	if err := json.Unmarshal(body, &result); err != nil {
		return nil, fmt.Errorf("failed to parse response JSON: %v", err)
	}
	// Check if the response contains errors
	if errors, exists := result["errors"]; exists {
		errorList, ok := errors.([]interface{})
		if !ok {
			return nil, fmt.Errorf("unexpected error format in response")
		}

		// Handle each error in the "errors" array
		for _, e := range errorList {
			errorDetails, ok := e.(map[string]interface{})
			if !ok {
				continue
			}

			// Get the error message (if available)
			if message, ok := errorDetails["message"].(string); ok {
				return nil, fmt.Errorf("%s", message)
			}
		}
	}
	return result, nil
}

// LoadEnv loads environment variables from a .env file
func LoadEnv() error {
	file, err := os.Open(".env")
	if err != nil {
		return err
	}
	defer file.Close()

	// Parse .env file
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		if len(line) == 0 || line[0] == '#' {
			continue
		}
		parts := strings.SplitN(line, "=", 2)
		if len(parts) != 2 {
			continue
		}
		os.Setenv(parts[0], parts[1])
	}

	if err := scanner.Err(); err != nil {
		return err
	}
	return nil
}
