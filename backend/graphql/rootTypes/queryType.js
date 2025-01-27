import { GraphQLObjectType } from 'graphql';
import userQuery from '../queries/userQuery.js';
import usersQuery from '../queries/usersQuery.js';
import highScoreQuery from '../queries/highScoreQuery.js';
import getTop10ScoresQuery from '../queries/top10ScoresQuery.js';

const queryType = new GraphQLObjectType({
    name: "Query",
    fields: {
        user: userQuery,
        users: usersQuery,
        highScore: highScoreQuery,
        top10Scores: getTop10ScoresQuery
    },
});


export default queryType;