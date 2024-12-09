#include "spmv.h"
#include "my_COO.h"
#include <stdio.h>
#include <stdlib.h>

COO convert_dense_to_COO(const unsigned int n, const double mat[]){
    COO coo;
    // Count the number of values in the matrix to create the arrays
    unsigned int size_indices = n;
    unsigned int size_values = 0;
    for (unsigned int i = 0; i < n * n; i++) {
        if (mat[i] != 0.0) {
            size_values++;
        }
    }
    coo.row_indices = (unsigned int *)malloc((size_values) * sizeof(unsigned int)); // n+1 for the last offset
    coo.column_indices = (unsigned int *)malloc(size_values * sizeof(unsigned int));
    coo.values = (double *)malloc(size_values * sizeof(double));
    coo.size_values = size_values;
    coo.size_indices = size_indices;
    unsigned int buffer = 0;
    for (unsigned int i = 0; i < n; i++){
        for (unsigned int j = 0; j < n ; j++){
            if (mat[i * n + j] !=0){
                coo.row_indices[buffer] = i;
                coo.column_indices[buffer] = j;
                coo.values[buffer] = mat[i * n + j];
                buffer++;
            }
        }
    }
    return coo;
}

void free_COO(COO *coo) {
    free(coo->row_indices);
    free(coo->column_indices);
    free(coo->values);
}

int my_sparse_COO(COO *coo, double vec[], double result[]) {
    for (unsigned int i = 0; i < coo->size_indices; i++) {
        result[i] = 0.0;
    }
    for (unsigned int i = 0; i < coo->size_values; i++) {
        result[coo->row_indices[i]] += coo->values[i] * vec[coo->column_indices[i]];
    }
    return 0;
}

void print_COO(COO coo) {
    printf("Column Indices:\n");
    for (unsigned int i = 0; i < coo.size_values; i++) {
        printf("%d ", coo.column_indices[i]);
    }
    printf("\nRow Indices:\n");
    for (unsigned int i = 0; i < coo.size_values; i++) {
        printf("%d ", coo.row_indices[i]);
    }
    printf("\nValues:\n");
    for (unsigned int i = 0; i < coo.size_values; i++) {
        printf("%.2f ", coo.values[i]);
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
    //Column Indices:
    //0 1 2 1 1 3 0 1 2 3 2 4 
    //Row Indices:
    //0 0 0 1 2 2 3 3 3 3 4 4 
    //Values:
    //2.00 3.00 5.00 4.00 2.00 10.00 2.00 5.00 8.00 16.00 17.00 14.00 
    
    COO coo = convert_dense_to_COO(n, mat);

    print_COO(coo);

    free(coo.column_indices);
    free(coo.row_indices);
    free(coo.values);

    return 0;
}
*/