#include <stdio.h>
#include <stdlib.h>
#include "vector.h"


// Returns a pointer to the new cloned matrix
int **cloneMatrix(int **matrix, int length, int width)
{

    int **clone = (int **)malloc(sizeof(int*) * length);
    for (int i = 0; i < length; i++)
    {
        clone[i] = (int *)malloc(sizeof(int) * width);
        for (int j = 0; j < width; j++)
        {
            clone[i][j] = matrix[i][j];
        }
    }

    return clone;
}

// Returns a pointer to the new cloned vector
int *cloneVector(int *vector, int size)
{

    int *clone = (int *)malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
    {
        clone[i] = vector[i];
    }
    return clone;
}

// Returns -1 if vector1 is greater than vector2, 0 if both are equal, and 1 if vector2 if greater
// Vector1 is greater than vector2 if and only if vector1[i] >= vector2[i] for all i and vice versa
int compareVectors(int *vector1, int *vector2, int length)
{
    int vector1_greater = 1;  
    int vector2_greater = 1;            
    
    for (int i = 0; i < length; i++) {
        // Check if vector1 is not >= vector2 at any position
        if (vector1[i] < vector2[i]) {
            vector1_greater = 0;
        }
        
        // Check if vector2 is not >= vector1 at any position
        if (vector2[i] < vector1[i]) {
            vector2_greater = 0;
        }
    }

    if (vector1_greater) {
        return -1;  // Vector1 is greater
    } else if (vector2_greater) {
        return 1; // Vector2 is greater
    } else {
        return 0; // Consider the vectors equal
    }
}

// Returns a pointer to a new matrix which is the sum of both matricies passed in
int **addMatricies(int **matrix1, int **matrix2, int length, int width)
{
    int **result = (int **)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
    {
        result[i] = (int *)malloc(sizeof(int) * width);
        for (int j = 0; j < width; j++)
        {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    return result;
}

// Returns a pointer to a new vector which is the sum of both vectors passed in
int *addVectors(int *vector1, int *vector2, int length)
{
    int *result = (int *)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++)
    {
        result[i] = vector1[i] + vector2[i];
    }

    return result;
}

void printMatrix(int **matrix, int length, int width)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < width; j++)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

void printVector(int *vector, int length)
{
    for (int i = 0; i < length; i++){
        printf("%d\t", vector[i]);
    }
    printf("\n");
}