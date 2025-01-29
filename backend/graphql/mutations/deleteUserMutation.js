import {GraphQLBoolean, GraphQLInt} from 'graphql';
import db from '../../models/index.js';

const deleteUserResolver = async (_, args, context) => {
    try{
        const isAuthorized = !!context.user_id
    
        if (!isAuthorized) {
            throw new Error("User not authenticated or user_id not found in context.");
        }

        const user = await db.User.findOne({
            where: {
                id: context.user_id,
            }
        })

        if (!user) {
            return false;
        }

        await user.destroy();
        return true;
  } catch (error) {
    console.error("Error deleting account:", error);
    throw new Error("Unable to delete account.");
  }
}

const deleteUserMutation = {
    type: GraphQLBoolean,
    resolve: deleteUserResolver,
};

export default deleteUserMutation;