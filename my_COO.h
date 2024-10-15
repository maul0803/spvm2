//
// Created by cytech on 02/10/24.
//

#ifndef MY_COO_H
#define MY_COO_H

typedef struct {
    unsigned int *row_indices;
    unsigned int *column_indices;
    double *values;
    unsigned int size_values;
    unsigned int size_indices;
} COO;

COO convert_dense_to_COO(unsigned int n, const double mat[]);
void free_COO(COO *coo);
int my_sparse_COO(COO *coo, double vec[], double result[]);
void print_COO(COO coo);

#endif
