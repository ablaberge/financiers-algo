#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

/**
 * Dijkstra's Banker's Algorithm isSafe() implementation. Determines if a given state is safe
 * based on a state file passed in by the user via main.c
 *
 * @param   available       Pointer to a vector containing the # of currently available resources
 * @param   alloc_matrix    Pointer to a 2D matrix specifying the # of rsources currently allocated to each process/thread
 * @param   need_matrix     Pointer to a 2D matrix specifying the # of resources each process needs to finish
 * @param   length          The number of threads/processes (also the length of the matricies)
 * @param   width           The number of resources (also the width of the matricies)
 */
void isSafe(int *available, int **alloc_matrix, int **need_matrix, int length, int width)
{

    int *work = cloneVector(available, width);

    int *finish = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
    {
        finish[i] = 0;
    }

    int num_finished = 0;
    int found = 1;

    while (found && num_finished < length)
    {
        found = 0; // Keeps track of if at least one process in this iteration could finish

        for (int i = 0; i < length; i++)
        {
            if (!finish[i]) // Check if each thread's resource needs can be satisfied
            {
                int can_execute = 1;
                for (int j = 0; j < width; j++)
                {
                    if (need_matrix[i][j] > work[j])
                    {
                        can_execute = 0; // Not enough available resources for this thread right now
                        break;
                    }
                }

                if (can_execute) // Thread i can finish so reclaim its resources
                {
                    for (int j = 0; j < width; j++)
                    {
                        work[j] += alloc_matrix[i][j];
                    }

                    finish[i] = 1;
                    num_finished++;
                    found = 1;
                }
            }
        }
    }

    int safe = (num_finished == length);
    if (safe)
    {
        for (int i = 0; i < length; i++)
        {
            finish[i] = 0;
        }

        int *schedule = (int *)malloc(sizeof(int) * length);
        int *schedule_avail = cloneVector(available, width); // Clone available for schedule finding since it will be modified
        findSafeSchedules(schedule_avail, alloc_matrix, need_matrix, length, width, finish, schedule, 0);

        free(schedule);
        free(schedule_avail);
    }
    else
    {
        printf("UNSAFE: ");
        for (int i = 0; i < length; i++)
        {
            if (!finish[i])
            {
                printf("T%d ", i);
            }
        }
        printf("can't finish\n");
    }

    free(work);
    free(finish);
}

/**
 * Called by isSafe() when we have identified at least once safe schedule for a given state file.
 * Recursively finds and prints all safe schedules for the processes.
 *
 * @param   available       Pointer to a vector containing the # of currently available resources
 * @param   alloc_matrix    Pointer to a 2D matrix specifying the # of rsources currently allocated to each process/thread
 * @param   need_matrix     Pointer to a 2D matrix specifying the # of resources each process needs to finish
 * @param   length          The number of threads/processes (also the length of the matricies)
 * @param   width           The number of resources (also the width of the matricies)
 * @param   finish          Pointer to a vector indicating which processes have completed
 * @param   schedule        Pointer to a vector containing the safe schedule for each recursive iteration
 * @param   step            Tracks the current position in the schedule being built (acts as a recursion depth counter)
 */
void findSafeSchedules(int *available, int **alloc_matrix, int **need_matrix, int length, int width, int *finish, int *schedule, int step)
{
    // Base case: all threads are finished
    if (step == length)
    {
        printf("SAFE: ");
        for (int i = 0; i < length; i++)
        {
            printf("T%d ", schedule[i]);
        }
        printf("\n");
        return;
    }

    for (int i = 0; i < length; i++)
    {
        if (!finish[i]) // Check if thread can execute with current available resources
        {
            int can_execute = 1;
            for (int j = 0; j < width; j++)
            {
                if (need_matrix[i][j] > available[j])
                {
                    can_execute = 0;
                    break;
                }
            }

            if (can_execute)
            {
                int *temp_available = cloneVector(available, width); // Don't modify current available state
                for (int j = 0; j < width; j++)
                {
                    available[j] += alloc_matrix[i][j];
                }

                finish[i] = 1;
                schedule[step] = i;

                // Recursively find next thread to execute
                findSafeSchedules(available, alloc_matrix, need_matrix, length, width, finish, schedule, step + 1);

                // Restore state
                finish[i] = 0;
                for (int j = 0; j < width; j++)
                {
                    available[j] = temp_available[j];
                }

                free(temp_available);
            }
        }
    }
}