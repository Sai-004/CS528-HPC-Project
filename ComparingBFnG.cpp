#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>
#include <fstream>
#include <ctime> // Include <ctime> for time()

using namespace std;

// Constants for maximum number of edge servers and time slots
const int MAX_SERVERS = 2;
const int MAX_TIMESLOTS = 10;

int serverEnery[MAX_SERVERS][MAX_TIMESLOTS]; // Solar panel capacity
int serverTasks[MAX_SERVERS][MAX_TIMESLOTS]; // Number of tasks arriving
int arrivedTasks[MAX_TIMESLOTS];   // Task arrival per timeslot

// Global variables for number of time slots, edge servers, and maximum energy per time slot per server
int T = 0;    // Number of time slots
int M = 0;    // Number of edge servers
int Smax = 0; // Max energy that can be generated in one time slot per server

// Function to implement a greedy algorithm to assign tasks to servers
int greedy(vector<int> task_arrived, vector<vector<int>> S)
{
    // 2D vector to store cumulative solar power for each server and time slot
    vector<vector<int>> cumulative_S(M, vector<int>(T, 0));

    // Calculate cumulative solar power for each server and time slot
    for (int i = 0; i < M; ++i)
    {
        for (int j = 0; j < T; ++j)
        {
            if (j == 0)
            {
                cumulative_S[i][j] = S[i][j];
            }
            else
            {
                cumulative_S[i][j] += cumulative_S[i][j - 1] + S[i][j];
            }
        }
    }

    // Flag to check if at least one execution is done in each iteration
    bool isAtleastOneExecutionDone = true;

    // Vector to store the energy utilizations of previous time slots for each server
    vector<int> previousStagesEnergyUtilisations(M, 0);

    // Matrix to store the number of tasks actually executed by each server in each time slot
    vector<vector<int>> tasksActuallyExecuted(M, vector<int>(T, 0));

    // Loop until no task can be executed in any time slot
    while (isAtleastOneExecutionDone)
    {
        isAtleastOneExecutionDone = false;

        vector<int> min_cummulativeSum(M, 0);
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < T; j++)
            {
                if (j == 0)
                {
                    min_cummulativeSum[i] = cumulative_S[i][j];
                }
                else
                {
                    if (cumulative_S[i][j] < min_cummulativeSum[i])
                    {
                        min_cummulativeSum[i] = cumulative_S[i][j];
                    }
                }
            }
        }

        // Iterate over each time slot
        for (int j = 0; j < T; j++)
        {
            // Reset previous stage energy utilizations at the beginning of each time slot
            if (j == 0)
            {
                for (int i = 0; i < M; i++)
                {
                    // cumulative_S[i][j] = min(cumulative_S[i][j], cumulative_S[i][j] - previousStagesEnergyUtilisations[i]);
                    previousStagesEnergyUtilisations[i] = 0;
                }
            }

            // Iterate over each server in each time slot
            for (int i = 0; i < M; i++)
            {
                // For the first time slot
                if (j == 0)
                {
                    // Check if there are tasks arrived and there is enough energy to execute the task
                    if (task_arrived[j] > 0 && cumulative_S[i][j] - (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3)) >= 0)
                    {
                        task_arrived[j]--;
                        cumulative_S[i][j] -= (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3));
                        previousStagesEnergyUtilisations[i] = (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3));
                        tasksActuallyExecuted[i][j]++;
                        isAtleastOneExecutionDone = true;
                    }
                    else if (task_arrived[j] == 0)
                        break; // No tasks left to be executed so go to next time step
                }
                else
                {
                    cumulative_S[i][j] -= previousStagesEnergyUtilisations[i];
                    if (task_arrived[j] > 0 && (cumulative_S[i][j] - (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3))) >= 0)
                    {
                        task_arrived[j]--;
                        int p = j;
                        int leftAfterUtilisation = cumulative_S[i][j] - (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3)); // energy left after utilisation
                        while (p >= 0 && cumulative_S[i][p] >= leftAfterUtilisation)
                        {
                            cumulative_S[i][p] = leftAfterUtilisation; // previous stages also should reflect the current utilisations in energy, if their energy was used
                            p--;
                        }
                        previousStagesEnergyUtilisations[i] += (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3));
                        tasksActuallyExecuted[i][j]++;
                        isAtleastOneExecutionDone = true;
                    }
                    else if (task_arrived[j] == 0)
                        break;
                }
            }
        }
    }

    // Calculate total number of tasks executed from tasksActuallyExecuted
    int final_tasks = 0;
    for (int j = 0; j < T; j++)
    {
        for (int i = 0; i < M; i++)
        {
            final_tasks += tasksActuallyExecuted[i][j];
            // cout << tasksActuallyExecuted[i][j] << " ";
        }
        // cout << "\n";
    }
    return final_tasks;
}

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
        for (int i = 0; i < M; i++)
        {
            battery[i] += serverEnery[i][time];
        }
        remaining_tasks = arrivedTasks[time];
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

// Function to generate a random test scenario
void generateRandomTest()
{
    vector<int> greedy_result;
    vector<int> brute_force_result;
    ofstream outfile("data.txt");


    for (int temp = 0; temp < 1000; temp++)
    {
        // Generate random number of edge servers, time slots, and max solar power per time slot per server
        M = rand() % MAX_SERVERS + 1;                // Random number of edge servers between 1 and 20
        T = rand() % MAX_TIMESLOTS + 1;              // Random number of time slots between 1 and 100
        Smax = rand() % 30 + 1;                      // Random max solar power per time slot per server
        vector<vector<int>> S(M, vector<int>(T, 0)); // Solar panel capacity
        vector<vector<int>> D(M, vector<int>(T, 0)); // Number of tasks arriving
        vector<int> Task_arrived(T, 0);              // Task arrival per timeslot
        cout << "Edge Servers: " << M << ", Time Slots: " << T << ", Max Solar Power: " << Smax << endl;

        // Generate random solar power generation and task arrival for each server and time slot
        for (int i = 0; i < M; ++i)
        {
            for (int j = 0; j < T; ++j)
            {
                S[i][j] = rand() % Smax + 1; // Random solar power between 1 and Smax
                serverEnery[i][j] = S[i][j];
                D[i][j] = rand() % 10 + 1;   // Random number of tasks between 1 and 10
                serverTasks[i][j] = D[i][j];
                // cout << "Server " << i + 1 << ", Time Slot " << j + 1 << ": Solar Power " << S[i][j] << ", Tasks " << D[i][j] << endl;
            }
        }

        // Calculate total number of tasks arriving in each time slot
        for (int i = 0; i < T; i++)
        {
            for (int j = 0; j < M; j++)
            {
                Task_arrived[i] += D[j][i];
                arrivedTasks[i] = Task_arrived[i];
            }
        }

        // Call greedy algorithm function and display the result
        int greedy_max_tasks = greedy(Task_arrived, S);
        // cout << "Final max greedy Tasks: " << greedy_max_tasks << "\n";
        greedy_result.push_back(greedy_max_tasks);

        vector<int> battery(MAX_SERVERS, 0);

        int brute_force_max_tasks = dp(0, 0, battery, 1);
        // cout << "Final max bruteforce Tasks: " << brute_force_max_tasks << endl;
        brute_force_result.push_back(brute_force_max_tasks);

        outfile << "Test Case " << temp + 1 << ":\n";
        outfile << "Greedy: " << greedy_max_tasks << "\n";
        outfile << "Brute Force: " << brute_force_max_tasks << "\n";
        cout << "Test Case " << temp + 1 << ":\n";
        cout << "Greedy: " << greedy_max_tasks << "\n";
        cout << "Brute Force: " << brute_force_max_tasks << "\n";
    }
    outfile.close();
}

// Main function
int main()
{
    generateRandomTest(); // Generate random test scenario
    return 0;
}
