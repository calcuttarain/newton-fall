import { JWT_SECRET } from '../../constants.js';
import loginInputType from '../types/loginInputType.js';
import loginResultType from '../types/loginResultType.js';
import jwt from 'jsonwebtoken';
import db from '../../models/index.js';
import bcrypt from 'bcrypt';

const loginMutationResolver = async (_, args) => {
    const user = await db.User.findOne({
        where: {
            name: args.credentials.username,
        }
    });

    if(!user) {
        return {
            token: null,
        }
    }

    const providedPassword = args.credentials.password;
    const userPassword = user.password;

    const passwordIsValid = await bcrypt.compare(providedPassword, userPassword);

    if(!passwordIsValid) {
        console.log("Password is invalid");

        return {
            token: null,
        }
    }
    
    const token = jwt.sign({ user_id: user.id }, JWT_SECRET);
    
    return {
        token,
    };
}

const loginMutation = {
    type: loginResultType,
    args: {
        credentials: {type: loginInputType},
    },
    resolve: loginMutationResolver,
};

export default loginMutation;