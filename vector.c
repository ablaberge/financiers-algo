#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

// TODO - Define vector and matrix operations

// Returns a pointer to the new cloned matrix
int **cloneMatrix(int **matrix, int length, int width) {

    int **clone = (int**)malloc(sizeof(int)*length);
    for (int i = 0; i < length; i++){
        clone[i] = (int*)malloc(sizeof(int)*width);
        for (int j = 0; j < width; j++){
            clone[i][j]=matrix[i][j];
        }
    }

    return clone;

}

// Returns a pointer to the new cloned vector
int *cloneVector(int *vector, int size) {

int *clone = (int*)malloc(sizeof(int)*size);

for (int i = 0; i < size; i++){
    clone[i]=vector[i];
}
return clone;

}

// Returns -1 if vector1 is greater than vector2, 0 if both are equal, and 1 if vector2 if greater
// Vector1 is greater than vector2 if and only if vector1[i] >= vector2[i] for all i and vice versa
int compareVectors(int *vector1, int *vector2){

    int comparator = 0;
    int counter = 0;

    while (vector1[counter] != NULL && vector2[counter] != NULL){
        if (vector1[counter] >= vector2[counter]){
            comparator--; 
        }
        if (vector1[counter] <= vector2[counter]){
            comparator++;
        }

        counter++;
    }

    if (comparator == abs(counter)) { // One of the vectors is greater
        if (comparator > 1) {
            comparator = 1;
            return comparator;
        }
        else {
            comparator = -1;
            return comparator;
        }
    }
    else {
        comparator = 0;
        return comparator;
    }


}

// Returns a pointer to a new matrix which is the sum of both matricies passed in
int **addMatricies (int **matrix1, int **matrix2){
    

}

// Returns a pointer to a new vector which is the sum of both vectors passed in
int *addVectors (int *vector1, int *vector2){

}

void printMatrix (int **matrix) {

}

void printVector (int *vector) {

}