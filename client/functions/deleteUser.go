package functions

import (
    "client/connect"
    "fmt"
)



func DeleteUser(){
    if(Token==""){
        fmt.Println("The user is not logged in!");
        return
    }
	query := `
    mutation delete{
       deleteUser
    }`
	variables := map[string]interface{}{
	}

	_, err := connect.GraphQLRequest(query, variables, Token)
	if err != nil {
		fmt.Println("Error:", err)
		return
	}

	
	fmt.Println("Succesfully deleted account")

}