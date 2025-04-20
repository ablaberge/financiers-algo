#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: bankers [state file]\n");
    printf("Please try again using the correct argument\n");
    return 1;
  }
  // Attempt to open scenario file and scan data into allocated structures
  FILE *pFile;
  pFile = fopen(argv[1], "r");
  if (pFile == NULL)
  {
    fclose(pFile);
    printf("fopen() failed\n");
    return 1;
  }
  int NRES;  // number of resource types
  int NPROC; // number of processes
  fscanf(pFile, "%d", &NRES);
  fscanf(pFile, "%d", &NPROC);

  int *total_avail = (int *)malloc(sizeof(int) * NRES);
  for (int i = 0; i < NRES; i++)
  {
    fscanf(pFile, "%d", &total_avail[i]);
  }

  int **max_matrix = (int **)malloc(sizeof(int *) * NPROC);
  for (int i = 0; i < NPROC; i++)
  {
    max_matrix[i] = (int *)malloc(sizeof(int) * NRES);
    for (int j = 0; j < NRES; j++)
    {
      fscanf(pFile, "%d", &max_matrix[i][j]);
    }
  }

  int **alloc_matrix = (int **)malloc(sizeof(int *) * NPROC);
  int **need_matrix = (int **)malloc(sizeof(int *) * NPROC);
  for (int i = 0; i < NPROC; i++)
  {
    alloc_matrix[i] = (int *)malloc(sizeof(int) * NRES);
    need_matrix[i] = (int *)malloc(sizeof(int) * NRES);
    for (int j = 0; j < NRES; j++)
    {
      fscanf(pFile, "%d", &alloc_matrix[i][j]);
      need_matrix[i][j] = max_matrix[i][j] - alloc_matrix[i][j];
    }
  }
  fclose(pFile);

  // Sanity check:
  int *total_allocated = (int *)malloc(sizeof(int) * NRES);
  int *available = (int *)malloc(sizeof(int) * NRES);

  for (int j = 0; j < NRES; j++)
  {
    total_allocated[j] = 0;
    for (int i = 0; i < NPROC; i++)
    {
      total_allocated[j] += alloc_matrix[i][j];
      if (alloc_matrix[i][j] > max_matrix[i][j]) // Ensure each thread’s needs do not exceed its max demands for each resource type.
      {
        printf("Integrity test failed: allocated resources exceed demand for Thread %d. ", i);
        printf("Need %d instances of resource %d. \n", max_matrix[i][j] - alloc_matrix[i][j], j);
        free(total_allocated);
        free(available);
        free(total_avail);
        for (int i = 0; i < NPROC; i++)
        {
          free(max_matrix[i]);
          free(alloc_matrix[i]);
          free(need_matrix[i]);
        }

        free(max_matrix);
        free(alloc_matrix);
        free(need_matrix);

        return 1;
      }
    }
    for (int j = 0; j < NRES; j++)
    {
      available[j] = total_avail[j] - total_allocated[j];
    }

    if (total_allocated[j] > total_avail[j])
    {
      printf("Integrity test failed: allocated resources exceed total resources\n");
      free(total_allocated);
      free(available);
      free(total_avail);
      for (int i = 0; i < NPROC; i++)
      {
        free(max_matrix[i]);
        free(alloc_matrix[i]);
        free(need_matrix[i]);
      }

      free(max_matrix);
      free(alloc_matrix);
      free(need_matrix);
      return 1;
    }
  }

  free(total_allocated);

  isSafe(available, alloc_matrix, need_matrix, NPROC, NRES);

  free(available);
  free(total_avail);
  for (int i = 0; i < NPROC; i++)
  {
    free(max_matrix[i]);
    free(alloc_matrix[i]);
    free(need_matrix[i]);
  }

  free(max_matrix);
  free(alloc_matrix);
  free(need_matrix);

  return 0;
}