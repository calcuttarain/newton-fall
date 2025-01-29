import db from '../../models/index.js';
import { findOrCreateTags } from '../repositories/tagsRepository.js';

export const createPost = async (post, context) => {
    const createdPost = await db.Post.create({
        title: post.title,
        body: post.body,
        userId: context.user_id,
     });
 
     const tags = await findOrCreateTags(post.tags);
 
     await createdPost.addTags(tags);

     return createdPost;
}