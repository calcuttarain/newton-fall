import {
    GraphQLInputObjectType, 
    GraphQLString,
    GraphQLNonNull,
} from 'graphql'

const loginInputType = new GraphQLInputObjectType({
    name: 'LoginInput',
    fields: {
        username: { type: new GraphQLNonNull(GraphQLString) },
        password: { type: new GraphQLNonNull(GraphQLString) },
    }
});

export default loginInputType;