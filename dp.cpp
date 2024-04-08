#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

const int MAX_EDGE_SERVERS = 100;
const int MAX_TIME_SLOTS = 24;

int M, T; // Number of edge servers and time slots
int S[MAX_EDGE_SERVERS][MAX_TIME_SLOTS]; // Solar panel capacity
int D[MAX_EDGE_SERVERS][MAX_TIME_SLOTS]; // Task arrivals
int dp[MAX_EDGE_SERVERS][MAX_TIME_SLOTS][MAX_EDGE_SERVERS * MAX_TIME_SLOTS]; // DP table

// Function to maximize the total number of task executions
int maximizeTasks() {
    // Initialize DP table
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < T; ++j) {
            for (int k = 0; k <= T * M; ++k) {
                dp[i][j][k] = -1;
            }
        }
    }

    // Base case: If no tasks, return 0
    for (int i = 0; i < M; ++i) {
        if (D[i][0] == 0) dp[i][0][0] = 0;
    }

    // Dynamic Programming
    for (int j = 0; j < T - 1; ++j) {
        for (int i = 0; i < M; ++i) {
            for (int k = 0; k <= T * M; ++k) {
                if (dp[i][j][k] == -1) continue;

                // Option 1: Execute current task
                if (k + pow(D[i][j], 3) <= S[i][j]) {
                    dp[i][j + 1][k + pow(D[i][j], 3)] = max(dp[i][j + 1][k + pow(D[i][j], 3)], dp[i][j][k] + 1);
                }

                // Option 2: Do not execute current task
                dp[i][j + 1][k] = max(dp[i][j + 1][k], dp[i][j][k]);

                // Option 3: Use stored power
                if (k >= pow(D[i][j], 3)) {
                    dp[i][j + 1][k - pow(D[i][j], 3)] = max(dp[i][j + 1][k - pow(D[i][j], 3)], dp[i][j][k]);
                }
            }
        }
    }

    // Find maximum tasks executed across all edge servers at the last time slot
    int maxTasks = 0;
    for (int i = 0; i < M; ++i) {
        maxTasks = max(maxTasks, dp[i][T - 1][0]);
    }

    return maxTasks;
}

int main() {
    // Input number of edge servers and time slots
    cin >> M >> T;

    // Input solar panel capacity
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < T; ++j) {
            cin >> S[i][j];
        }
    }

    // Input task arrivals
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < T; ++j) {
            cin >> D[i][j];
        }
    }

    // Calculate and output the maximum number of tasks executed
    cout << maximizeTasks() << endl;

    return 0;
}
