package functions

import (
    "client/connect"
    "fmt"
)

var Token string = ""

func Login(name, password string){
	query := `
	mutation login($user: String!, $pass: String!){
        login(credentials: { username: $user, password: $pass }) {
            token
        }
    }`
	variables := map[string]interface{}{
        "user":name,
        "pass":password,
	}

	response, err := connect.GraphQLRequest(query, variables, Token)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}
    data, ok := response["data"].(map[string]interface{})
	if !ok {
		fmt.Println("Error: Unable to parse response data")
		return
	}

	// Extract token from the response
	loginResponse, ok := data["login"].(map[string]interface{})
	if !ok {
		fmt.Println("Error: Unable to parse login response")
		return
	}

	// Here you can directly access the token
	token, ok := loginResponse["token"].(string)
	if !ok {
		fmt.Println("Error: Token not found in response")
		return
	}

	Token = token
	fmt.Println("Successfully logged in!")

}