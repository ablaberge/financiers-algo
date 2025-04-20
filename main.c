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
    return;
  }
  // Attempt to open scenario file and scan data into allocated structures
  FILE *pFile;
  pFile = fopen(argv[1], "r");
  if (pFile == NULL)
  {
    fclose(pFile);
    printf("fopen() failed\n");
    return;
  }
  int NRES;  // number of resource types
  int NPROC; // number of processes
  fscanf(pFile, "%d", &NRES);
  fscanf(pFile, "%d", &NPROC);

  int *available = (int *)malloc(sizeof(int) * NRES);
  for (int i = 0; i < NRES; i++)
  {
    fscanf(pFile, "%d", available[i]);
  }

  int **max_matrix = (int **)malloc(sizeof(int) * NPROC);
  for (int i = 0; i < NPROC; i++)
  {
    max_matrix[i] = (int *)malloc(sizeof(int) * NRES);
    for (int j = 0; j < NRES; j++)
    {
      fscanf(pFile, "%d", max_matrix[i][j]);
    }
  }

  int **alloc_matrix = (int **)malloc(sizeof(int) * NPROC);
  for (int i = 0; i < NPROC; i++)
  {
    alloc_matrix[i] = (int *)malloc(sizeof(int) * NRES);
    for (int j = 0; j < NRES; j++)
    {
      fscanf(pFile, "%d", alloc_matrix[i][j]);
    }
  }

  // Sanity check:

  for (int i = 0; i < NPROC; i++)
  {
    for (int j = 0; j < NRES; j++)
    {
      // Ensure that the currently allocated resources do not exceed the total number of resources.
      if (alloc_matrix[i][j] > available[i])
      {
        printf("Integrity test failed: allocated resources exceed total resources\n");
        return;
      }
      
      // Ensure each thread’s needs do not exceed its max demands for each resource type.
      if (alloc_matrix[i][j] > max_matrix[i][j])
      {
        printf("Integrity test failed: allocated resources exceed demand for Thread %d", i);
        printf("Need %d instances of resource %d",max_matrix[i][j] - alloc_matrix[i][j], j);
        return;
      }
    }
  }



  // TODO: Run banker's safety algorithm

  // TODO: return malloc'd mem (alloc_matrix, max_matrix, available vector)

  return 0;
}