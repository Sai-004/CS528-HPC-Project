# CS528-HPC-Project
## Task Scheduling in Edge Server Systems with Limited Solar Energy and Infinite Batteries

### Introduction

This project focuses on task scheduling optimization in edge server systems equipped with solar panels and batteries. The goal is to efficiently allocate tasks to maximize task executions throughout the day while considering solar power generation, task arrivals, and battery usage.

### Problem Statement

Given M edge servers with solar panels of capacity S and infinite batteries, the task is to schedule tasks arriving at different time slots of the day to maximize task execution. Each time slot has a certain amount of solar power generation and a number of tasks arriving at each edge server. The power consumed in a time slot is based on the cube of the number of tasks executed. The task can be executed in the same time slot or stored for later execution. 

### Brute Force Approach

The solution is implemented using Dynamic Programming (DP). The `dp` function recursively computes the maximum number of tasks that can be executed starting from a particular edge server and time slot, considering the available battery power and remaining tasks. Memoization is used to optimize computation by storing already computed results.

### Approach used was mentioned in the Report

### Usage

1. **Input:** Provide the number of edge servers (M), the number of time slots per day (T), solar power generation, and task arrival for each server and time slot.
2. **Output:** The maximum number of tasks executed throughout the day by all edge servers.

### Installation

1. Clone the repository:

    ```bash
    git clone <repository_url>
    ```

2. Compile the code:

    ```bash
    g++ -o final final.cpp
    ```

3. Run the program:

    ```bash
    ./final
    ```

### Example

An example input and output can be as follows:

```
Enter the number of edge servers: 3
Enter the number of time slots per day: 24
Enter solar power generation and task arrival for each server and time slot:
[Input your data here]

Max number of tasks executed: [Output number of tasks]
```

### Contributing

Contributions are welcome! Please feel free to open issues or pull requests for any improvements or suggestions.
