'use strict';

import fs from 'fs';
import path from 'path';
import Sequelize from 'sequelize';
import process from 'process';
import config from '../config/config.js';

const env = process.env.NODE_ENV || 'development';

let sequelize;
if (config.use_env_variable) {
  sequelize = new Sequelize(process.env[config.use_env_variable], config[env]);
} else {
  sequelize = new Sequelize(config[env].database, config[env].username, config[env].password, config[env]);
}

const models = await Promise.all(fs
  .readdirSync(import.meta.dirname)
  .filter(file => {
    return (
      file.indexOf('.') !== 0 &&
      file !== import.meta.dirname,
      file !== 'index.js' &&
      file.slice(-3) === '.js' &&
      file.indexOf('.test.js') === -1
    );
  })
  .map(async file => {
    const module = await import(path.join(import.meta.dirname, file));
    return module.default(sequelize, Sequelize.DataTypes);
  })
);

const db = models.reduce((acc, model) => {
  acc[model.name] = model;
  return acc;
}, {});

Object.keys(db).forEach(modelName => {
  if (db[modelName].associate) {
    db[modelName].associate(db);
  }
});

db.sequelize = sequelize;
db.Sequelize = Sequelize;

export default db;
