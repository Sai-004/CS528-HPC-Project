#include <iostream>
#include <vector>
#include <map>
#include <cstring>
using namespace std;

const int MAX_SERVERS = 105;
const int MAX_TIMESLOTS = 25;

int S[MAX_SERVERS][MAX_TIMESLOTS]; // Solar panel capacity
int D[MAX_SERVERS][MAX_TIMESLOTS]; // Number of tasks arriving
int Task_arrived[MAX_TIMESLOTS];    // Task arrival per timeslot

int T; // Number of time slots
int M; // Number of edge servers

int dp(int server, int time, vector<int> battery, int remaining_tasks, map<tuple<int, int, vector<int>, int>, int>& memo) {

    if (time >= T)
        return 0;
    if (server == M) {
        int result = dp(0, time + 1, battery, remaining_tasks, memo);
        return result;
    }
    if (server == 0) {
        for (int i = 0; i < MAX_SERVERS; i++) {
            battery[i] += S[i][time];
        }
        remaining_tasks = Task_arrived[time];
    }
    if (remaining_tasks == 0) {
        int result = dp(0, time + 1, battery, remaining_tasks, memo);
        return result;
    }

    // Check if the result is already computed
    if (memo.count({server, time, battery, remaining_tasks})) {
        return memo[{server, time, battery, remaining_tasks}];
    }

    int max_tasks = 0;
    for (int i = 0; i <= remaining_tasks; ++i) {
        int consumed_power = i * i * i;
        if (consumed_power > battery[server])
            break;

        battery[server] -= consumed_power;
        int temp_remaining_tasks = remaining_tasks - i;
        max_tasks = max(max_tasks, i + dp(server + 1, time, battery, temp_remaining_tasks, memo));
        battery[server] += consumed_power;
    }

    // Memoize the result
    memo[{server, time, battery, remaining_tasks}] = max_tasks;

    return max_tasks;
}

int main() {
    cout << "Enter the number of edge servers: ";
    cin >> M;
    cout << "Enter the number of time slots per day: ";
    cin >> T;

    cout << "Enter solar power generation and task arrival for each server and time slot:\n";
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < T; ++j) {
            cin >> S[i][j] >> D[i][j];
        }
    }

    for (int i = 0; i < T; i++) {
        for (int j = 0; j < M; j++) {
            Task_arrived[i] += D[j][i];
        }
    }

    vector<int> battery(MAX_SERVERS, 0);
    map<tuple<int, int, vector<int>, int>, int> memo;

    int max_tasks = dp(0, 0, battery, 1, memo);
    cout << "Max number of tasks executed: " << max_tasks << endl;

    return 0;
}
