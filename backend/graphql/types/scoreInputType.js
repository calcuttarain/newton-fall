import { GraphQLInputObjectType, GraphQLInt, GraphQLFloat } from 'graphql';

const scoreInputType = new GraphQLInputObjectType({
  name: 'ScoreInput',
  fields: {
    level: { type: GraphQLInt },
    distance: { type: GraphQLFloat },
    time: { type: GraphQLFloat },
    hpFinal: { type: GraphQLFloat },
    totalScore: { type: GraphQLFloat },
  },
});

export default scoreInputType;
