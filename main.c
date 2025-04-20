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

  int *available = (int *)malloc(sizeof(int) * NRES);
  for (int i = 0; i < NRES; i++)
  {
    fscanf(pFile, "%d", &available[i]);
  }

  int **max_matrix = (int **)malloc(sizeof(int*) * NPROC);
  for (int i = 0; i < NPROC; i++)
  {
    max_matrix[i] = (int *)malloc(sizeof(int) * NRES);
    for (int j = 0; j < NRES; j++)
    {
      fscanf(pFile, "%d", &max_matrix[i][j]);
    }
  }

  int **alloc_matrix = (int **)malloc(sizeof(int*) * NPROC);
  int **need_matrix = (int **)malloc(sizeof(int*) * NPROC);
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


  // Sanity check:

  int *total_allocated = (int *)malloc(sizeof(int) * NRES);

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
        return 1;
      }
    }

    if (total_allocated[j] > available[j])  // Check if total allocated exceeds the total available
    {
      printf("Integrity test failed: allocated resources exceed total resources\n");
      return 1;
    }
  }
  free(total_allocated);



  // TODO: Run banker's safety algorithm

  isSafe(available, alloc_matrix, need_matrix, NPROC, NRES);



  free(available);
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