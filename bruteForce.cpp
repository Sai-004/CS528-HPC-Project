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

// Recursive function to implement brute force algorithm
int bf(int server, int time, vector<int> battery, int remaining_tasks)
{
    // Base case: if we reached the end of time slots, return 0
    if (time >= T)
        return 0;
    
    // If we have iterated through all servers, move to the next time slot
    if (server == M)
    {
        int result = bf(0, time + 1, battery, remaining_tasks);
        return result;
    }
    
    // If we're at the beginning of a time slot, update battery and remaining tasks
    if (server == 0)
    {
        for (int i = 0; i < MAX_SERVERS; i++)
        {
            battery[i] += S[i][time];
        }
        remaining_tasks = Task_arrived[time];
    }
    
    // If no tasks left, move to the next time slot
    if (remaining_tasks == 0)
    {
        int result = bf(0, time + 1, battery, remaining_tasks);
        return result;
    }

    int max_tasks = 0;
    // Try all possible combinations of tasks for the current server
    for (int i = 0; i <= remaining_tasks; ++i)
    {
        int consumed_power = i * i * i;

        // If consumed power exceeds available battery, break
        if (consumed_power > battery[server])
            break;

        battery[server] -= consumed_power;
        int temp_remaining_tasks = remaining_tasks - i;

        // Recursively call for the next server
        max_tasks = max(max_tasks, i + bf(server + 1, time, battery, temp_remaining_tasks));
        battery[server] += consumed_power;
    }

    return max_tasks;
}

int main()
{
    // Input parameters from the user
    cout << "Enter the number of edge servers: ";
    cin >> M;
    cout << "Enter the number of time slots per day: ";
    cin >> T;
    cout << "Enter maximum solar power that can be generated in each time slot, in each server: ";
    cin >> Smax;
    cout << "Enter solar power generation and task arrival for each server and time slot:\n";

    // Input solar power generation and task arrival for each server and time slot
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < T; ++j)
        {
            cin >> S[i][j] >> D[i][j];
        }
    }

    // Calculate total tasks arriving in each time slot
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

    // Initialize battery vector with zeros
    vector<int> battery(MAX_SERVERS, 0);

    // Call brute force function to find the maximum tasks executed
    int max_tasks = bf(0, 0, battery, 1);
    cout << "Max number of tasks executed: " << max_tasks << endl;

    return 0;
}
