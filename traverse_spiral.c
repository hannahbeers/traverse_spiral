////////////////////////////////////////////////////////////////////////////////
// Main File:        traverse_spiral.c
// This File:        traverse_spiral.c
// Other Files:      traverse1.txt and traverse2.txt for testing
// Semester:         CS 354 Spring 2019
//
// Author:           Hannah Beers
// Email:            hlbeers@wisc.edu
// CS Login:         beers
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          None
//
// Online sources:   None
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *COMMA = ",";

// Structure representing Matrix
// size: Dimension of the square (size*size)
// array: 2D array of integers
typedef struct _Square {
    int size;
    int **array;
} Square;

/* Retrieves from the first line of the input file,
 * the size of the square matrix.
 *
 * fp: file pointer for input file
 * size: pointer to size of the square matrix
 */
void get_dimensions(FILE *fp, int *size) {
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}

/* Traverses a given layer from the square matrix
 *
 * array: Heap allocated 2D square matrix
 * size: size of the 2D square matrix
 * layer: layer number to be traversed
 * op: pointer to the output file
 */
void traverse_layer(int **array, int size, int layer, FILE *op) {
    int row_num, col_num;

    //corner case: size is odd & the layer is last so only one entry to print
    if(size % 2 == 1 && layer == (size + 1) / 2 - 1){
        fprintf(op, "%d ", *(*(array + layer) + layer));
        return;
    }

    //Traverse upper row from left to right with appropriate bounds
    row_num = layer;
    for (int i = layer; i < size - layer; i++) {
        fprintf(op, "%d ", *(*(array + row_num) + i));
    }

    //Traverse right column from top to bottom with appropriate bounds
    col_num = size - layer - 1;
    for (int i = layer + 1; i < size - layer - 1; i++) {
        fprintf(op, "%d ", *(*(array + i) + col_num));
    }

    //Traverse lower row from right to left with appropriate bounds
    row_num = size - layer - 1;
    for (int i = size - layer - 1; i >= layer; i--) {
        fprintf(op, "%d ", *(*(array + row_num) + i));
    }

    //Traverse left column from bottom to top with appropriate bounds
    col_num = layer;
    for (int i = size - layer - 2; i >= layer + 1; i--) {
        fprintf(op, "%d ", *(*(array + i) + col_num));
    }
}

/* Traverses the square matrix spirally
 *
 * square: pointer to a structure that contains 2D square matrix
 * op: pointer to the output file
 */
void traverse_spirally(Square *square, FILE *op) {
    int size = square->size;
    int num_layers = 0;
    num_layers = size/2;
    if(size%2 == 1) {
        num_layers++;
    }

    int i;
    for(i = 0; i < num_layers; i++) {
        traverse_layer(square->array, size, i, op);
    }
    fprintf(op, "\n");
}

/* This program reads a square matrix from the input file
 * and outputs its spiral order traversal to the output file
 *
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {

    //Check if number of command-line arguments is correct.
    if (argc != 3) {
        printf("Incorrect number of command-line arguments\n");
        exit(1);
    }

    //Open the input file and check if it opened successfully
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Cannot open input file for reading\n");
        exit(1);
    }

    //Declare local variables.
    //int size - the size of the square matrix
    int size;

    //Call the function get_dimensions to retrieve size of the square matrix
    get_dimensions(fp, &size);

    //Dynamically allocate a 2D array as per the retrieved dimensions
    int **arr = malloc(size * sizeof(int *));
    for(int i = 0; i < size; i++) {
        *(arr + i) = malloc(size * sizeof(int));
        //Check the return value of malloc()
        if (*(arr + i) == NULL) {
            printf("Error while checking the return value of malloc()\n");
            exit(1);
        }
    }

    //Read the file line by line by using the function getline as used in get_dimensions
    //Tokenize each line wrt comma to store the values in the square matrix
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file\n");
            exit(1);
        }

        token = strtok(line, COMMA);

        //Initialize your 2D array.
        for(int j = 0; j < size; j++) {
            *(*(arr + i) + j) = 0;
        }

        for(int j = 0; j < size; j++) {
            *(*(arr + i) + j) = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }

    //Create a structure and initialize its size and array fields appropriately
    Square *square = malloc(sizeof(Square));
    //Check the return value of malloc()
    if (square == NULL) {
        printf("Error while checking the return value of malloc()\n");
        exit(1);
    }
    square->array = arr;
    square->size = size;

    //Open the output file and check if it opened successfully
    FILE *fp2 = fopen(*(argv + 2), "w");
    if (fp2 == NULL) {
        printf("Cannot open output file for reading\n");
        exit(1);
    }

    //Call the function traverse_spirally
    traverse_spirally(square, fp2);

    //Free the dynamically allocated memory
    for (int i = 0; i < size; i++) {
        free(*(arr + i));
    }
    free(arr);
    arr = NULL;
    free(square);
    square = NULL;

    //Close the input file
    if (fclose(fp) != 0) {
        printf("Error while closing the input file\n");
        exit(1);
    }

    //Close the output file
    if (fclose(fp2) != 0) {
        printf("Error while closing the output file\n");
        exit(1);
    }

    return 0;
}