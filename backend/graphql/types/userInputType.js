import {GraphQLInputObjectType, GraphQLObjectType, GraphQLString} from 'graphql'

const userInputType = new GraphQLInputObjectType({
    name: 'UserInput',
    fields: {
        name: { type: GraphQLString },
        password: { type: GraphQLString }
    }
});

export default userInputType;