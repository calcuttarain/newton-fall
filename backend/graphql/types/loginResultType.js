import {GraphQLObjectType, GraphQLString} from 'graphql'

const loginResultType = new GraphQLObjectType({
    name: 'LoginResult',
    fields: {
        token: { type: GraphQLString },
    }
});

export default loginResultType;