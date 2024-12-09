#include "spmv.h"
#include "my_CSC.h"
#include <stdio.h>
#include <stdlib.h>

CSC convert_dense_to_CSC(const unsigned int n, const double mat[]){
    CSC csc;
    // Count the number of values in the matrix to create the arrays
    unsigned int size_column_offsets = n+1;
    unsigned int size_row_indices_values = 0;
    for (unsigned int i = 0; i < n * n; i++) {
        if (mat[i] != 0.0) {
            size_row_indices_values++;
        }
    }
    csc.column_offsets = (unsigned int *)malloc((size_column_offsets) * sizeof(unsigned int)); // n+1 for the last offset
    csc.row_indices = (unsigned int *)malloc(size_row_indices_values * sizeof(unsigned int));
    csc.values = (double *)malloc(size_row_indices_values * sizeof(double));
    csc.size_column_offsets = size_column_offsets;
    csc.size_row_indices_values = size_row_indices_values;
    unsigned int buffer = 0;
    /*for (unsigned int j = 0; j < n; j++){
        csc.column_offsets[j] = buffer;
        for (unsigned int i = 0; i < n ; i++){
            if (mat[i * n + j] !=0){
                csc.row_indices[buffer] = i;
                csc.values[buffer] = mat[i * n + j];
                buffer ++;
            }
        }
    }*/
    // Magic trick
    for (unsigned int j = 0; j < n; j++) {
        csc.column_offsets[j] = buffer;
        for (unsigned int i = 0; i < n; i++) {
            if (mat[i * n + j] != 0) {
                buffer++;
            }
        }
    }
    buffer = 0;
    for (unsigned int j = 0; j < n; j++) {
        for (unsigned int i = 0; i < n; i++) {
            if (mat[i * n + j] != 0) {
                csc.row_indices[buffer] = i;
                csc.values[buffer] = mat[i * n + j];
                buffer++;
            }
        }
    }
    csc.column_offsets[n] = buffer;
    return csc;
}

void free_CSC(CSC *csc) {
    free(csc->column_offsets);
    free(csc->row_indices);
    free(csc->values);
}

int my_sparse_CSC(CSC *csc, double vec[], double result[]) {
    for (unsigned int i = 0; i < csc->size_column_offsets - 1; i++) {
        result[i] = 0.0;
    }
    // Go through all columns
    for (unsigned int j = 0; j < csc->size_column_offsets - 1; j++) {
        for (unsigned int k = csc->column_offsets[j]; k < csc->column_offsets[j + 1]; k++) {
            result[csc->row_indices[k]] += csc->values[k] * vec[j]; // Select the row shortcut
        }
    }
    return 0;
}

void print_CSC(CSC csc) {
    printf("Column Offsets:\n");
    for (unsigned int i = 0; i < csc.size_column_offsets; i++) {
        printf("%u ", csc.column_offsets[i]);
    }
    printf("\nRow Indices:\n");
    for (unsigned int i = 0; i < csc.size_row_indices_values; i++) {
        printf("%u ", csc.row_indices[i]);
    }
    printf("\nValues:\n");
    for (unsigned int i = 0; i < csc.size_row_indices_values; i++) {
        printf("%.2f ", csc.values[i]);
    }
    printf("\n");
}

/*
int main() {
    unsigned int n = 5;
    double mat[25] = {
        2, 3, 5, 0, 0,
        0, 4, 0, 0, 0,
        0, 2, 0, 10, 0,
        2, 5, 8, 16, 0,
        0, 0, 17, 0, 14
    };
    //https://www.youtube.com/watch?v=a2LXVFmGH_Q
    //Column Offsets:
    //0 2 6 9 11 12 
    //Row Indices:
    //0 3 0 1 2 3 0 3 4 2 3 4
    //Values:
    //2.00 2.00 3.00 4.00 2.00 5.00 5.00 8.00 17.00 10.00 16.00 14.00 
    
    CSC csc = convert_dense_to_CSC(n, mat);

    print_CSC(csc);

    free(csc.column_offsets);
    free(csc.row_indices);
    free(csc.values);

    return 0;
}
*/