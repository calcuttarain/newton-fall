'use strict';
import { Model } from 'sequelize';

export default (sequelize, DataTypes) =>  {
  class Score extends Model {
    /**
     * Helper method for defining associations.
     * This method is not a part of Sequelize lifecycle.
     * The `models/index` file will call this method automatically.
     */
    static associate(models) {
      Score.belongsTo(models.User, { foreignKey: 'userId' });
    }
  }
  Score.init({
    userId: {
      type: DataTypes.INTEGER,
      allowNull: false,
    },
    level: DataTypes.INTEGER,
    distance: DataTypes.FLOAT,
    time: DataTypes.FLOAT,
    hpFinal: DataTypes.FLOAT,
    totalScore: DataTypes.FLOAT
  }, {
    sequelize,
    modelName: 'Score',
  });
  return Score;
};