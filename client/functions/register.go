package functions

import (
    "client/connect"
    "fmt"
)



func Register(name, password string){
	query := `
    mutation Register($user: String!, $pass: String!){
        createUser(user: {
            name: $user
            password: $pass
        }) {
            name
        }
    }`
	variables := map[string]interface{}{
        "user":name,
        "pass":password,
	}

	_, err := connect.GraphQLRequest(query, variables, Token)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}
    

	// Extract token from the response
	
	fmt.Println("Succesfully created account")

}