#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>
using namespace std;

// Constants for maximum number of edge servers and time slots
const int MAX_SERVERS = 20;
const int MAX_TIMESLOTS = 100;

// Global variables for number of time slots, edge servers, and maximum energy per time slot per server
int T;    // Number of time slots
int M;    // Number of edge servers
int Smax; // Max energy that can be generated in one time slot per server

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

    cout << "Cumulative Solar Power:\n";
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < T; j++)
        {
            cout << cumulative_S[i][j] << " ";
        }
        cout << endl;
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
                    //cumulative_S[i][j] = min(cumulative_S[i][j], cumulative_S[i][j] - previousStagesEnergyUtilisations[i]);
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
                        int p=j;
                        int leftAfterUtilisation=cumulative_S[i][j] - (pow((tasksActuallyExecuted[i][j] + 1), 3) - pow(tasksActuallyExecuted[i][j], 3));
                        while(p>=0 && cumulative_S[i][p]>=leftAfterUtilisation)
                        {
                            cumulative_S[i][p]=leftAfterUtilisation;//previous stages also should reflect the current utilisations in energy, if their energy was used
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
            cout << tasksActuallyExecuted[i][j] << " ";
        }
        cout << "\n";
    }
    return final_tasks;
}

// Function to generate a random test scenario
void generateRandomTest()
{
    srand(time(0)); // Seed for random number generation

    // Generate random number of edge servers, time slots, and max solar power per time slot per server
    M = rand() % MAX_SERVERS + 1;                // Random number of edge servers between 1 and 20
    T = rand() % MAX_TIMESLOTS + 1;              // Random number of time slots between 1 and 100
    Smax = rand() % 100 + 1;                     // Random max solar power per time slot per server
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
            D[i][j] = rand() % 10 + 1;   // Random number of tasks between 1 and 10
            cout << "Server " << i + 1 << ", Time Slot " << j + 1 << ": Solar Power " << S[i][j] << ", Tasks " << D[i][j] << endl;
        }
    }

    // Calculate total number of tasks arriving in each time slot
    for (int i = 0; i < T; i++)
    {
        for (int j = 0; j < M; j++)
        {
            Task_arrived[i] += D[j][i];
        }
    }

    // Call greedy algorithm function and display the result
    int greedy_max_tasks = greedy(Task_arrived, S);
    cout << "Final max greedy Tasks: " << greedy_max_tasks << "\n";
}

// Main function
// int main()
// {
//     generateRandomTest(); // Generate random test scenario
//     return 0;
// }

int main()
{
    cout << "Enter the number of edge servers: ";
    cin >> M;
    cout << "Enter the number of time slots per day: ";
    cin >> T;
    cout << "Enter maximum solar power that can be generated in each time slot, in each server: ";
    cin >> Smax;
    cout << "Enter solar power generation and task arrival for each server and time slot:\n";
    vector<vector<int>> S(M, vector<int>(T, 0)); // Solar panel capacity
    vector<vector<int>> D(M, vector<int>(T, 0)); // Number of tasks arriving
    vector<int> Task_arrived(T, 0);              // Task arrival per timeslot
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
            // if((Task_arrived[i]+D[j][i])>(M*pow(T*Smax,1.0/3.0))) break;
            Task_arrived[i] += D[j][i];
        }
    }

    int greedy_max_tasks = greedy(Task_arrived, S);
    cout << "Final max greedy Tasks: " << greedy_max_tasks << "\n";

    return 0;
}