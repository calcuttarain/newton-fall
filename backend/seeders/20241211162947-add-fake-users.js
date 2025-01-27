'use strict';
const { faker } = require('@faker-js/faker');
const bcrypt = require("bcrypt");

/** @type {import('sequelize-cli').Migration} */
module.exports = {
  async up (queryInterface, Sequelize) {
    const password = await bcrypt.hash('aaaa', 5);
    const mockUsers = new Array(100).fill().map(() => {
      return {
        name: faker.internet.username(),
        password: password,
        createdAt: Date(),
        updatedAt: Date(),
      }
    });

    await queryInterface.bulkInsert('Users', mockUsers, {});
  },

  async down (queryInterface, Sequelize) {
    /**
     * Add commands to revert seed here.
     *
     * Example:
     * await queryInterface.bulkDelete('People', null, {});
     */
  }
};
