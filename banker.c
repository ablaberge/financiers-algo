#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

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
        found = 0;

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
        for (int i = 0; i < length; i++) {
            finish[i] = 0;
        }
        
        int *schedule = (int *)malloc(sizeof(int) * length);
        int *schedule_avail = cloneVector(available, width);  // Clone available for schedule finding since it will be modified
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
                int *temp_available = (int *)malloc(width * sizeof(int)); // Save current state before modifying
                for (int j = 0; j < width; j++)
                {
                    temp_available[j] = available[j];
                    available[j] += alloc_matrix[i][j];
                }

                finish[i] = 1;
                schedule[step] = i;

                // Recursively find next thread to execute
                findSafeSchedules(available, alloc_matrix, need_matrix,length, width, finish, schedule, step + 1);

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