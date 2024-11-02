#include <iostream>
#include <vector>
#include <limits.h>
#include <algorithm>
#include <queue>
#include <unordered_map>

struct Job
{
    int id;
    int processing_time;
    int release_date;
    int inventory_change;
};

struct State
{
    int time;
    int inventory;
    int makespan;
    bool operator<(const State &other) const
    {
        return makespan > other.makespan;
    }
};

class Scheduler
{
public:
    Scheduler(std::vector<Job> &jobs, int inventory_capacity, int initial_inventory)
        : jobs(jobs), inventory_capacity(inventory_capacity), initial_inventory(initial_inventory) {}

    int findOptimalMakespan()
    {
        int left = 0, right = calculateMaxTime();
        int optimal_makespan = -1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (isFeasible(mid))
            {
                optimal_makespan = mid;
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }
        return optimal_makespan;
    }

private:
    std::vector<Job> jobs;
    int inventory_capacity;
    int initial_inventory;

    // Maximum time bound calculation
    int calculateMaxTime()
    {
        int max_time = 0;
        for (const auto &job : jobs)
        {
            max_time += job.processing_time;
        }
        return max_time;
    }

    // Check feasibility for a given makespan using DP and a graph representation
    bool isFeasible(int makespan)
    {
        std::priority_queue<State> pq;
        pq.push({0, initial_inventory, 0}); // Starting state

        std::unordered_map<int, std::unordered_map<int, int>> visited; // Track visited states

        while (!pq.empty())
        {
            State current = pq.top();
            pq.pop();

            if (current.time > makespan)
                continue;
            if (visited[current.time][current.inventory] && visited[current.time][current.inventory] <= current.makespan)
            {
                continue;
            }
            visited[current.time][current.inventory] = current.makespan;

            // Process all jobs
            for (const auto &job : jobs)
            {
                if (current.time < job.release_date)
                    continue;

                int new_time = current.time + job.processing_time;
                int new_inventory = current.inventory + job.inventory_change;

                // Check inventory constraints
                if (new_inventory < 0 || new_inventory > inventory_capacity)
                    continue;

                // Push new state to priority queue
                pq.push({new_time, new_inventory, std::max(current.makespan, new_time)});
            }
        }

        // Check if a valid sequence was found within the makespan
        for (auto &entry : visited[makespan])
        {
            if (entry.second <= makespan)
            {
                return true;
            }
        }
        return false;
    }
};

int main()
{
    std::vector<Job> jobs = {
        {1, 5, 0, -3}, {2, 3, 2, 2}, {3, 4, 6, -2}, {4, 2, 8, 1}};
    int inventory_capacity = 10;
    int initial_inventory = 5;

    Scheduler scheduler(jobs, inventory_capacity, initial_inventory);
    int min_makespan = scheduler.findOptimalMakespan();

    if (min_makespan != -1)
    {
        std::cout << "Minimum makespan: " << min_makespan << std::endl;
    }
    else
    {
        std::cout << "No feasible solution found." << std::endl;
    }

    return 0;
}