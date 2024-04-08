#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int MAX_SERVERS = 105;
const int MAX_TIMESLOTS = 25;

int S[MAX_SERVERS][MAX_TIMESLOTS]; // Solar panel capacity
int D[MAX_SERVERS][MAX_TIMESLOTS]; // Number of tasks arriving
int Task_arrived[MAX_TIMESLOTS]; // Task arrival per timeslot
int memo[MAX_SERVERS][MAX_TIMESLOTS];

int T; // Number of time slots
int M; // Number of edge servers

int dp(int server, int time, vector<int> battery, int remaining_tasks) {

    if (time >= T)
        return 0;
    if(server==M)
    {
        int result = dp(0, time + 1, battery, remaining_tasks);
        //cout << "Returned from dp: server = " << server << ", time = " << time << ", remaining_tasks = " << remaining_tasks << ", result = " << result << endl;
        return result;
    }
    if(server == 0)
    {
        for (int i = 0; i < MAX_SERVERS;i++)
        {
            battery[i]+=S[i][time];
        }
        remaining_tasks = Task_arrived[time];
    }
    if (remaining_tasks == 0)
    {
        int result = dp(0, time + 1, battery, remaining_tasks);
        //cout << "Returned from dp: server = " << server << ", time = " << time << ", remaining_tasks = " << remaining_tasks << ", result = " << result << endl;
        return result;
    }
    
    //cout << "Debug: server = " << server << ", time = " << time << ", remaining_tasks = " << remaining_tasks << endl;
    int max_tasks = 0;
    for (int i = 0; i <= remaining_tasks; ++i)
    {
        int consumed_power = i * i * i;
        if (consumed_power > battery[server]) // If consumed power exceeds available power
            break;
        
        battery[server] -= consumed_power;
        int temp_remaining_tasks = remaining_tasks - i;
        max_tasks = max(max_tasks, i + dp(server+1, time,battery,temp_remaining_tasks));
        battery[server] += consumed_power;
    }
    //cout << "Returned from dp: server = " << server << ", time = " << time << ", remaining_tasks = " << remaining_tasks << ", max_tasks = " << max_tasks << endl;
    return max_tasks;
}

int main() {
    // Input S, D, T, M
    // For simplicity, assume the inputs are read
    cout << "Enter the number of edge servers: ";
    cin >> M;
    cout << "Enter the number of time slots per day: ";
    cin >> T;

    // Input solar power generation and task arrival for each server and time slot
    cout << "Enter solar power generation and task arrival for each server and time slot:\n";
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < T; ++j) {
            cin >> S[i][j] >> D[i][j];
        }
    }

    for (int i = 0; i < T; i++)
    {
        for (int j = 0; j < M; j++)
        {
            Task_arrived[i] += D[j][i];
        }
    }
    //memset(memo, -1, sizeof(memo));
    vector<int> battery(MAX_SERVERS,0); // Battery capacity at each server

    int max_tasks = dp(0, 0, battery, 1); // Start from the first server and first time slot
    cout << "Max number of tasks executed: " << max_tasks << endl;
    
    return 0;
}
