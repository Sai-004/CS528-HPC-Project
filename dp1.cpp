#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>
using namespace std;

const int MAX_SERVERS = 4;
const int MAX_TIMESLOTS = 100;

int T; // Number of time slots
int M; // Number of edge servers
int Smax; //Max energy that can be generated in one time slot per server

int dp(int timeSlot,int energy_remaining,vector<int>&task_remaining,vector<int>&arrived_energy,vector<int>&final_task_remaining,vector<vector<int>>& memo)
{

    if(timeSlot>=T)
    {
        //cout << "DEBUG: Base case reached. Returning 0." << endl;
        return 0;
    }
    //cout << "DEBUG: dp called with timeSlot=" << timeSlot << ", energy_remaining=" << energy_remaining << endl;
    if(memo[timeSlot][energy_remaining]!=-1)
    {
        return memo[timeSlot][energy_remaining];
    }
    energy_remaining+=arrived_energy[timeSlot];
    int max_tasks = 0;
    int max_tasks_done=0;
    for (int i = 0; i <= task_remaining[timeSlot]; ++i) {
        int consumed_power = i * i * i;
        if (consumed_power > energy_remaining)
            break;
        energy_remaining-=consumed_power;
        task_remaining[timeSlot]-=i;
        int temp =0;
        if(timeSlot<(T-1))
        {
            if(memo[timeSlot+1][energy_remaining]!=-1)
            {
                temp = memo[timeSlot+1][energy_remaining];
            }
            else
            {
                temp = i+ dp(timeSlot+1,energy_remaining,task_remaining,arrived_energy,final_task_remaining,memo);
            }
        }
        else
        {
            temp = i+ dp(timeSlot+1,energy_remaining,task_remaining,arrived_energy,final_task_remaining,memo);
        }
        if(max_tasks<temp)
        {
            max_tasks=temp;
            max_tasks_done = i;
        }
        energy_remaining+=consumed_power;
        task_remaining[timeSlot]+=i;
    }
    final_task_remaining[timeSlot]=task_remaining[timeSlot]- max_tasks_done;
    //cout << "DEBUG: Returning max_tasks=" << max_tasks << " for timeSlot=" << timeSlot <<"Energy: "<< energy_remaining<<"task_remaining[timeSlot] :"<<task_remaining[timeSlot]<<endl;
    return memo[timeSlot][energy_remaining] =  max_tasks;
}

int allservers(vector<int> task_remaining,vector<vector<int>> &Sorted_S)
{
    //cout << "DEBUG: allservers called" << endl;
    vector<vector<int>> memo(T,vector<int>(T*(Smax+1),-1));
    vector<int> temp_remaining(T,0);
    cout<<"Task_Remaining: ";
    for(int i=0;i<T;i++)
    {
        cout<<task_remaining[i]<<" ";
    }
    cout<<"\n";

    int max_tasks=0;
    for(int i=0;i<M;i++)
    {
        cout << "Calling dp for server " << i << endl;
        max_tasks+= dp(0,0,task_remaining,Sorted_S[i],temp_remaining,memo);
        task_remaining=temp_remaining;
        cout << "Done dp for server " << i << endl;

        for(int k = 0; k < T; k ++)
        {
            for(int j = 0; j <T*(Smax+1); j ++)
            {
                memo[k][j]=-1;
            }
        }
    }
    return max_tasks;
}

void generateRandomTest() {
    srand(time(0)); // Seed for random number generation

    M = rand() % MAX_SERVERS + 1; // Random number of edge servers between 1 and 100
    // T = rand() % MAX_TIMESLOTS + 1;  // Random number of time slots between 1 and 20
    T = 50;
    Smax = rand() % 27 + 1; // Random max solar power per time slot per server
    int S[M][T]; // Solar panel capacity
    vector<vector<int>> D(M,vector<int>(T,0));// Number of tasks arriving
    vector<int> Task_arrived(T,0);    // Task arrival per timeslot
    cout << "Edge Servers: " << M << ", Time Slots: " << T << ", Max Solar Power: " << Smax << endl;

    // Generating random solar power generation and task arrival for each server and time slot
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < T; ++j) {
            S[i][j] = rand() % Smax + 1; // Random solar power between 1 and Smax
            D[i][j] = rand() % 3 + 1; // Random number of tasks between 1 and 10
            cout << "Server " << i+1 << ", Time Slot " << j+1 << ": Solar Power " << S[i][j] << ", Tasks " << D[i][j] << endl;
        }
    }

    for (int i = 0; i < T; i++) {
        for (int j = 0; j < M; j++) {
            // if((Task_arrived[i]+D[j][i])>(M*pow(T*Smax,1.0/3.0))) break;
            Task_arrived[i] += D[j][i];
        }
    }
    map<int, vector<int>> energyMap; // Map to store total energy and server indices with the same energy
    vector<vector<int>> Sorted_S;
    for (int i = 0; i < M; ++i) {
        int totalEnergy = 0;
        for (int j = 0; j < T; ++j) {
            totalEnergy += S[i][j];
        }
        energyMap[totalEnergy].push_back(i); // Store total energy and server index in the map
    }
    vector<int> temp(T,0);
    cout << "Sorted solar panel capacities based on total energy:\n";
    for (auto it = energyMap.begin(); it != energyMap.end(); ++it) {
        int totalEnergy = it->first;
        vector<int>& serverIndices = it->second;
        for (int serverIndex : serverIndices) {
            for(int i=0;i<T;i++)
            {
                temp[i]=S[serverIndex][i];
            }
            Sorted_S.push_back(temp);
        }
    }
    for (int i = 0; i < M; ++i) {
        cout<<"index : "<<i<<" ";
        for (int j = 0; j < T; ++j) {
            cout<< Sorted_S[i][j]<< " ";
        }
        cout<<"\n";
    }
    int max_tasks = allservers(Task_arrived,Sorted_S);
    cout<<"Final max Tasks: "<<max_tasks<<"\n";
    
}

int main()
{
    generateRandomTest();
    return 0;
}

// int main() {
//     cout << "Enter the number of edge servers: ";
//     cin >> M;
//     cout << "Enter the number of time slots per day: ";
//     cin >> T;
//     cout << "Enter maximum solar power that can be generated in each time slot, in each server: ";
//     cin >> Smax;
//     cout << "Enter solar power generation and task arrival for each server and time slot:\n";
//     int S[M][T]; // Solar panel capacity
//     vector<vector<int>> D(M,vector<int>(T,0));// Number of tasks arriving
//     vector<int> Task_arrived(T,0);    // Task arrival per timeslot
//     for (int i = 0; i < M; ++i) {
//         for (int j = 0; j < T; ++j) {
//             cin >> S[i][j] >> D[i][j];
//         }
//     }

//     for (int i = 0; i < T; i++) {
//         for (int j = 0; j < M; j++) {
//             //if((Task_arrived[i]+D[j][i])>(M*pow(T*Smax,1.0/3.0))) break;
//             Task_arrived[i] += D[j][i];
//         }
//     }

//     map<int, vector<int>> energyMap; // Map to store total energy and server indices with the same energy
//     vector<vector<int>> Sorted_S;
//     for (int i = 0; i < M; ++i) {
//         int totalEnergy = 0;
//         for (int j = 0; j < T; ++j) {
//             totalEnergy += S[i][j];
//         }
//         energyMap[totalEnergy].push_back(i); // Store total energy and server index in the map
//     }
//     vector<int> temp(T,0);
//     cout << "Sorted solar panel capacities based on total energy:\n";
//     for (auto it = energyMap.begin(); it != energyMap.end(); ++it) {
//         int totalEnergy = it->first;
//         vector<int>& serverIndices = it->second;
//         for (int serverIndex : serverIndices) {
//             for(int i=0;i<T;i++)
//             {
//                 temp[i]=S[serverIndex][i];
//             }
//             Sorted_S.push_back(temp);
//         }
//     }
//     for (int i = 0; i < M; ++i) {
//         cout<<"index : "<<i<<" ";
//         for (int j = 0; j < T; ++j) {
//             cout<< Sorted_S[i][j]<< " ";
//         }
//         cout<<"\n";
//     }
//     int max_tasks = allservers(Task_arrived,Sorted_S);
//     cout<<"Final max Tasks: "<<max_tasks<<"\n";
//     return 0;
// }
