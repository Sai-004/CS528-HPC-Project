#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>
using namespace std;

const int MAX_SERVERS = 105;
const int MAX_TIMESLOTS = 25;

int S[MAX_SERVERS][MAX_TIMESLOTS]; // Solar panel capacity
int D[MAX_SERVERS][MAX_TIMESLOTS]; // Number of tasks arriving
int Task_arrived[MAX_TIMESLOTS];   // Task arrival per timeslot

int T;    // Number of time slots
int M;    // Number of edge servers
int Smax; // Max energy that can be generated in one time slot per server

int dp(int server, int time, vector<int> battery, int remaining_tasks)
{

    if (time >= T)
        return 0;
    if (server == M)
    {
        int result = dp(0, time + 1, battery, remaining_tasks);
        return result;
    }
    if (server == 0)
    {
        for (int i = 0; i < MAX_SERVERS; i++)
        {
            battery[i] += S[i][time];
        }
        remaining_tasks = Task_arrived[time];
    }
    if (remaining_tasks == 0)
    {
        int result = dp(0, time + 1, battery, remaining_tasks);
        return result;
    }

    int max_tasks = 0;
    for (int i = 0; i <= remaining_tasks; ++i)
    {
        int consumed_power = i * i * i;
        if (consumed_power > battery[server])
            break;

        battery[server] -= consumed_power;
        int temp_remaining_tasks = remaining_tasks - i;
        max_tasks = max(max_tasks, i + dp(server + 1, time, battery, temp_remaining_tasks));
        battery[server] += consumed_power;
    }

    return max_tasks;
}

int main()
{
    cout << "Enter the number of edge servers: ";
    cin >> M;
    cout << "Enter the number of time slots per day: ";
    cin >> T;
    cout << "Enter maximum solar power that can be generated in each time slot, in each server: ";
    cin >> Smax;
    cout << "Enter solar power generation and task arrival for each server and time slot:\n";
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < T; ++j)
        {
            cin >> S[i][j] >> D[i][j];
        }
    }

    for (int i = 0; i < T; i++)
    {
        for (int j = 0; j < M; j++)
        {
            // as only limited amount of battery can be generated max number of tasks that can be executed in any
            // time slot is given below.If more than that number of tasks arrive, rest of the taks can be ignored
            // as they will never be scheduled any way.
            if ((Task_arrived[i] + D[j][i]) > (M * pow(T * Smax, 1.0 / 3.0)))
                break;
            Task_arrived[i] += D[j][i];
        }
    }

    vector<int> battery(MAX_SERVERS, 0);

    int max_tasks = dp(0, 0, battery, 1);
    cout << "Max number of tasks executed: " << max_tasks << endl;

    return 0;
}
