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

void schedule(vector<vector<int>> S, vector<vector<int>> D)
{
    vector<vector<int>> assigned(M, vector<int>(T,0));
    cout<<'\n';
    int num_task=1;

    while(true)
    {
        bool isAtleastOneExecutionDone = false;
        for(int i=0; i<T; i++)
        {
            int cumulative = 0;
            for(int j=0; j<M; j++)
            {
                cumulative += D[j][i];
            }

            for(int j=0; j<M; j++)
            {
                int powerRequired = num_task*num_task*num_task;
                int currentPower = powerRequired - (assigned[j][i]*assigned[j][i]*assigned[j][i]); // Calculate the remaining power needed

                if (currentPower <= S[j][i])
                {
                    S[j][i] -= currentPower; // Update the energy generated
                    assigned[j][i] += 1; // Assign the task
                    isAtleastOneExecutionDone = true; // Set the flag indicating execution
                }
                else
                {
                    int sum = 0;
                    bool isPossible = false;
                    vector<pair<int,int>> taken;

                    for(int k=i; k>=0; k--)
                    {
                        if(sum + S[j][k] >= currentPower)
                        {
                            isPossible = true;
                            taken.push_back(make_pair(k, currentPower - sum));
                            break;
                        }
                        else
                        {
                            taken.push_back(make_pair(k, S[j][k]));
                        }
                    }

                    if(isPossible)
                    {
                        for(auto [a,b] : taken)
                        {
                            S[j][a] -= b;
                        }
                        assigned[j][i] += 1;
                        isAtleastOneExecutionDone = true;
                    }                    
                }
            }
        }

        num_task++;
        if(!isAtleastOneExecutionDone)
        {
            break; // Exit the loop if no execution was done in this iteration
        }
    }

    int totalTasks = 0;
    for(int i=0; i<M; i++)
    {
        for(int j=0; j<T; j++)
        {
            cout<<"Server "<<i+1<<" on time slot "<<j+1<<" executed "<<assigned[i][j]<<" tasks\n";
            totalTasks += assigned[i][j];
        }
    }
    cout<<"Total executed tasks: "<<totalTasks<<'\n';
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
    schedule(S,D);
    return 0;
}