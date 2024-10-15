//
// Created by cytech on 02/10/24.
//

#ifndef MY_CSC_H
#define MY_CSC_H


typedef struct {
    unsigned int *column_offsets;
    unsigned int *row_indices;
    double *values;
    unsigned int size_column_offsets;
    unsigned int size_row_indices_values;
} CSC;

CSC convert_dense_to_CSC(unsigned int n, const double mat[]);
double* convert_CSC_to_dense(CSC csc, unsigned int n);
void free_CSC(CSC *csc);
int my_sparse_CSC(CSC *csc, double vec[], double result[]);
void print_CSC(CSC csc);
#endif
