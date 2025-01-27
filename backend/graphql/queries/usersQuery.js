import { GraphQLList } from 'graphql';
import userType from '../types/userType.js';
import db from '../../models/index.js';

const usersQueryResolver = async () => {
    const users = await db.User.findAll();

    return users;
}

const usersQuery = {
    type: new GraphQLList(userType),
    resolve: usersQueryResolver,
};

export default usersQuery;