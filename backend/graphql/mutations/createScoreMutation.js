import db from '../../models/index.js';
import scoreType from '../types/scoreType.js';
import scoreInputType from '../types/scoreInputType.js';

const createScoreMutationResolver = async (_, { score }, context) => {
  try {
    const isAuthorized = !!context.user_id
   
    if (!isAuthorized) {
        throw new Error("User not authenticated or user_id not found in context.");
    }

    const createdScore = await db.Score.create({
      userId: context.user_id,
      level: score.level,
      distance: score.distance,
      time: score.time,
      hpFinal: score.hpFinal,
      totalScore: score.totalScore,
    });

    return createdScore;
  } catch (error) {
    console.error("Error creating score:", error);
    throw new Error("Unable to create score.");
  }
};

const createScoreMutation = {
  type: scoreType,
  args: {
    score: { type: scoreInputType },
  },
  resolve: createScoreMutationResolver,
};

export default createScoreMutation;
