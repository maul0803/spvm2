//
// Created by cytech on 02/10/24.
//

#ifndef MY_CSR_H
#define MY_CSR_H

typedef struct {
    unsigned int *row_offsets;
    unsigned int *column_indices;
    double *values;
    unsigned int size_row_offsets;
    unsigned int size_column_indices_values;
} CSR;

CSR convert_dense_to_CSR(unsigned int n, const double mat[]);
double* convert_CSR_to_dense(CSR csr, unsigned int n);
void free_CSR(CSR *csr);
int my_sparse_CSR(CSR *csr, double vec[], double result[]);

#endif
