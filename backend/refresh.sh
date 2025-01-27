#!/bin/bash
rm -f ./db.sqlite
npx sequelize db:migrate
npx sequelize db:seed:all 