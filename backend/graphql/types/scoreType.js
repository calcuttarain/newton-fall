import { GraphQLObjectType, GraphQLInt, GraphQLFloat } from 'graphql';
import userType from './userType.js';
import db from "../../models/index.js";

const scoreType = new GraphQLObjectType({
  name: 'Score',
  fields: {
    id: { type: GraphQLInt },
    user: {
        type: userType,
        resolve: async (score) => {
            const user = await db.User.findOne({
            where: {
                id: score.userId
            },
        });

        return user;
    }},
    level: { type: GraphQLInt },
    distance: { type: GraphQLFloat },
    time: { type: GraphQLFloat },
    hpFinal: { type: GraphQLFloat },
    totalScore: { type: GraphQLFloat },
  },
});

export default scoreType;
