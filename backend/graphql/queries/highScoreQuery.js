import db from '../../models/index.js';
import scoreType from '../types/scoreType.js';
import { GraphQLInt } from 'graphql';

const highScoreResolver = async (_, args, context) => {
  try {
    const userId = context.user_id;

    if (!userId) {
      throw new Error("User not authenticated or user_id not found in context.");
    }


    const maxScore = await db.Score.findOne({
        where: { 
            userId, 
            level: args.level
          },
          order: [['totalScore', 'DESC']],
    });

    if (!maxScore) {
      throw new Error("No scores found for this user.");
    }

    return maxScore;
  } catch (error) {
    console.error("Error fetching max score:", error);
    throw new Error("Unable to fetch max score. Please try again.");
  }
};

const highScoreQuery = {
  type: scoreType,
  args: {
    level: { type: GraphQLInt },
  },
  resolve: highScoreResolver,
};

export default highScoreQuery;
