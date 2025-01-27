import db from '../../models/index.js';
import scoreType from '../types/scoreType.js';
import { GraphQLInt, GraphQLList } from 'graphql';

const getTop10ScoresResolver = async (_,args) => {
  try {
    // Query to find the top 10 scores ordered by totalScore in descending order
    const topScores = await db.Score.findAll({
      limit: 10,
      where: {level: args.level},
      order: [['totalScore', 'DESC']],
    });

    if (!topScores.length) {
      throw new Error("No scores found.");
    }

    return topScores;
  } catch (error) {
    console.error("Error fetching top 10 scores:", error);
    throw new Error("Unable to fetch top 10 scores. Please try again.");
  }
};

const getTop10ScoresQuery = {
  type: new GraphQLList(scoreType), 
  args: {
    level: { type: GraphQLInt },
  },
  resolve: getTop10ScoresResolver,
};

export default getTop10ScoresQuery;