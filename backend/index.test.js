import { test, describe, beforeEach } from 'node:test';
import assert from 'node:assert/strict';
import http from 'node:http';

const PORT = 3001;
let authToken = null;

// Function to send GraphQL queries
const queryGraphql = (query, variables = {}) => {
    return new Promise((resolve, reject) => {
        const postData = JSON.stringify({ query, variables });
        const options = {
            method: 'POST',
            hostname: 'localhost',
            port: PORT,
            path: '/graphql',
            headers: {
                'Content-Type': 'application/json',
                'Content-Length': Buffer.byteLength(postData),
                ...(authToken ? { Authorization: `Bearer ${authToken}` } : {}),
            },
        };

        const req = http.request(options, (res) => {
            let body = '';
            res.on('data', (chunk) => {
                body += chunk;
            });
            res.on('end', () => {
                resolve(JSON.parse(body));
            });
        });

        req.on('error', reject);
        req.write(postData);
        req.end();
    });
};

describe('GraphQL API Tests', () => {
  beforeEach(async () => {
    authToken = null; // Reset auth token

    // Log in first to ensure deleteUser works
    await queryGraphql(`
        mutation {
            login(credentials: { username: "Test", password: "aaaa" }) {
                token
            }
        }
    `).then(response => {
        if (response.data && response.data.login.token) {
            authToken = response.data.login.token;
        }
    }).catch(() => {}); // Ignore errors if user doesn't exist

    // Now try deleting the user
    await queryGraphql(`
        mutation {
            deleteUser
        }
    `).catch(() => {}); // Ignore errors if user doesn't exist
});


    test('Create User', async () => {
        const response = await queryGraphql(`
            mutation {
                createUser(user: {
                    name: "Test",
                    password: "aaaa"
                }) {
                    id
                    name
                }
            }
        `);

        assert.ok(response.data.createUser.id, 'User ID should be returned');
        assert.equal(response.data.createUser.name, 'Test', 'User name should be Test');
    });

    test('Login User', async () => {
        await queryGraphql(`
            mutation {
                createUser(user: {
                    name: "Test",
                    password: "aaaa"
                }) {
                    id
                    name
                }
            }
        `);

        const response = await queryGraphql(`
            mutation {
                login(credentials: { username: "Test", password: "aaaa" }) {
                    token
                }
            }
        `);

        assert.ok(response.data.login.token, 'Token should be returned');
        authToken = response.data.login.token;
    });

    test('Create Score (Authorized)', async () => {
        await queryGraphql(`
            mutation {
                createUser(user: {
                    name: "Test",
                    password: "aaaa"
                }) {
                    id
                }
            }
        `);

        const loginResponse = await queryGraphql(`
            mutation {
                login(credentials: { username: "Test", password: "aaaa" }) {
                    token
                }
            }
        `);
        authToken = loginResponse.data.login.token;

        const response = await queryGraphql(`
            mutation {
                createScore(score: {
                    level: 1
                    distance: 1000
                    time: 100
                    hpFinal: 100
                    totalScore: 10000
                }) {
                    id
                    level
                    distance
                    time
                    hpFinal
                    totalScore
                }
            }
        `);

        assert.ok(response.data.createScore.id, 'Score ID should be returned');
        assert.equal(response.data.createScore.level, 1, 'Score level should be 1');
        assert.equal(response.data.createScore.totalScore, 10000, 'Total score should be 10000');
    });

    test('Query High Score (Requires Login)', async () => {
      // Create a user and log in to get a token
      await queryGraphql(`
          mutation {
              createUser(user: {
                  name: "Test",
                  password: "aaaa"
              }) {
                  id
              }
          }
      `);
  
      const loginResponse = await queryGraphql(`
          mutation {
              login(credentials: { username: "Test", password: "aaaa" }) {
                  token
              }
          }
      `);
      authToken = loginResponse.data.login.token;
  
      // Ensure a high score exists before querying
      await queryGraphql(`
          mutation {
              createScore(score: {
                  level: 1
                  distance: 100
                  time: 100
                  hpFinal: 100
                  totalScore: 1000
              }) {
                  id
              }
          }
      `);
  
      // Now query high scores with authentication
      const response = await queryGraphql(`
          query {
              highScore(level: 1) {
                  level
                  distance
                  time
                  hpFinal
                  totalScore
              }
          }
      `);
  
      assert.ok(response.data, 'Response data should not be null');
      assert.ok(response.data.highScore, 'High score should be returned');
      assert.equal(response.data.highScore.level, 1, 'Level should be 1');
      assert.ok(Number.isInteger(response.data.highScore.totalScore), 'Total score should be an integer');
  });

    test('Query Top 10 Scores', async () => {
        const response = await queryGraphql(`
            query {
                top10Scores(level: 1) {
                    id
                    level
                    totalScore
                }
            }
        `);

        assert.ok(Array.isArray(response.data.top10Scores), 'Top 10 scores should be an array');
    });

    test('Delete User (Requires Login)', async () => {
        await queryGraphql(`
            mutation {
                createUser(user: {
                    name: "Test",
                    password: "aaaa"
                }) {
                    id
                }
            }
        `);

        const loginResponse = await queryGraphql(`
            mutation {
                login(credentials: { username: "Test", password: "aaaa" }) {
                    token
                }
            }
        `);
        authToken = loginResponse.data.login.token;

        const deleteResponse = await queryGraphql(`
            mutation {
                deleteUser
            }
        `);

        assert.equal(deleteResponse.data.deleteUser, true, 'User should be deleted');
    });
    test('Fail to Create User with Existing Username', async () => {
      await queryGraphql(`
          mutation {
              createUser(user: {
                  name: "Test",
                  password: "aaaa"
              }) {
                  id
              }
          }
      `);
  
      const response = await queryGraphql(`
          mutation {
              createUser(user: {
                  name: "Test",
                  password: "aaaa"
              }) {
                  id
              }
          }
      `);
  
      assert.ok(response.errors, 'Error should be returned when creating a user with an existing name');
      assert.strictEqual(response.data.createUser, null, 'createUser should be null when failing');
  });
  
  test('Fail to Login with Incorrect Credentials', async () => {
      await queryGraphql(`
          mutation {
              createUser(user: {
                  name: "Test",
                  password: "aaaa"
              }) {
                  id
              }
          }
      `);
  
      const response = await queryGraphql(`
          mutation {
              login(credentials: { username: "Test", password: "wrongpassword" }) {
                  token
              }
          }
      `);
  
      assert.ok(response.data, 'Response should contain data');
      assert.ok(response.data.login, 'Response should contain login field');
      assert.strictEqual(response.data.login.token, null, 'Token should be null for failed login');
  });
  
  
  
  test('Fail to Create Score Without Login', async () => {
      authToken = null; // Ensure no authentication
  
      const response = await queryGraphql(`
          mutation {
              createScore(score: {
                  level: 1
                  distance: 1000
                  time: 100
                  hpFinal: 100
                  totalScore: 10000
              }) {
                  id
              }
          }
      `);
  
      assert.ok(response.errors, 'Error should be returned when trying to create a score without login');
  });
  
  test('Fail to Query High Score Without Login', async () => {
      authToken = null; // Ensure no authentication
  
      const response = await queryGraphql(`
          query {
              highScore(level: 1) {
                  level
                  distance
                  time
                  hpFinal
                  totalScore
              }
          }
      `);
  
      assert.ok(response.errors, 'Error should be returned when querying high scores without login');
  });
  
});
