#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <gsl/gsl_spmatrix.h>
//#include <gsl/gsl_vector.h>
#include "timer.h"
#include "spmv.h"
#include "my_CSR.h"
#include "my_CSC.h"
#include "my_COO.h"

//#include <gsl/gsl_blas.h>
//#include <gsl/gsl_spblas.h>

// #include <mkl.h>  // Intel MKL mpicc
#include <gsl/gsl_cblas.h>      // CBLAS in GSL (the GNU Scientific Library) gcc
#define DEFAULT_SIZE 16384
#define DEFAULT_DENSITY 0.1

unsigned int populate_sparse_matrix(double mat[], unsigned int n, double density, unsigned int seed)
{
  unsigned int nnz = 0;

  srand(seed);

  for (unsigned int i = 0; i < n * n; i++) {
    if ((rand() % 100) / 100.0 < density) {
      // Get a pseudorandom value between -9.99 e 9.99
      mat[i] = ((double)(rand() % 10) + (double)rand() / RAND_MAX) * (rand() % 2 == 0 ? 1 : -1);
      nnz++;
    } else {
      mat[i] = 0;
    }
  }

  return nnz;
}

unsigned int populate_vector(double vec[], unsigned int size, unsigned int seed)
{
  srand(seed);

  for (unsigned int i = 0; i < size; i++) {
    vec[i] = ((double)(rand() % 10) + (double)rand() / RAND_MAX) * (rand() % 2 == 0 ? 1 : -1);
  }

  return size;
}

int is_nearly_equal(double x, double y)
{
  const double epsilon = 1e-5 /* some small number */;
  return fabs(x - y) <= epsilon * fabs(x);
  // see Knuth section 4.2.2 pages 217-218
}

unsigned int check_result(double ref[], double result[], unsigned int size)
{
  for(unsigned int i = 0; i < size; i++) {
    if (!is_nearly_equal(ref[i], result[i]))
      return 0;
  }

  return 1;
}

int main(int argc, char *argv[])
{
  int size;        // number of rows and cols (size x size matrix)
  double density;  // aprox. ratio of non-zero values

  if (argc < 2) {
    size = DEFAULT_SIZE;
    density = DEFAULT_DENSITY;
  } else if (argc < 3) {
    size = atoi(argv[1]);
    density = DEFAULT_DENSITY;
  } else {
    size = atoi(argv[1]);
    density = atoi(argv[2]);
  }

  double *mat, *vec, *refsol, *mysol;

  mat = (double *) malloc(size * size * sizeof(double));
  vec = (double *) malloc(size * sizeof(double));
  refsol = (double *) malloc(size * sizeof(double));
  mysol = (double *) malloc(size * sizeof(double));

  unsigned int nnz = populate_sparse_matrix(mat, size, density, 1);
  populate_vector(vec, size, 2);

  printf("Matriz size: %d x %d (%d elements)\n", size, size, size*size);
  printf("%d non-zero elements (%.2lf%%)\n\n", nnz, (double) nnz / (size*size) * 100.0);
  //
  // Dense computation using CBLAS (eg. GSL's CBLAS implementation)
  //
  //printf("CBLAS dense computation\n----------------\n");

  timeinfo start, now;
  timestamp(&start);

  cblas_dgemv(CblasRowMajor, CblasNoTrans, size, size, 1.0, mat, size, vec, 1, 0.0, refsol, 1);

  timestamp(&now);
  printf("Time taken by CBLAS dense computation: %ld ms\n", diff_milli(&start, &now));

  //
  // Using your own dense implementation
  //
  //printf("\nMy Own Dense computation\n----------------\n");
  timestamp(&start);

  my_dense(size, mat, vec, mysol);

  timestamp(&now);
  printf("Time taken by my dense matrix-vector product: %ld ms\n", diff_milli(&start, &now));

  if (check_result(refsol, mysol, size) == 1)
    printf("Result my dense is ok!\n");
  else
    printf("Result my dense is wrong!\n");
  
  // Your own sparse COO implementation
  //
  //printf("\nMy Own Spare COO computation\n----------------\n");

  COO per_mat_coo = convert_dense_to_COO(size, mat);
  // Compare times (and computation correctness!)
  timestamp(&start);
  my_sparse_COO(&per_mat_coo, vec, mysol);
  timestamp(&now);
  printf("Time taken by my own sparse matrix COO - vector product: %ld ms\n", diff_milli(&start, &now));
  if (check_result(refsol, mysol, size) == 1)
      printf("Result own spare COO implementation is ok!\n");
  else
      printf("Result own spare COO implementation is wrong!\n");

  // Your own sparse CSR implementation
  //
  //printf("\nMy Own Spare CSR computation\n----------------\n");

  CSR per_mat_csr = convert_dense_to_CSR(size, mat);
  // Compare times (and computation correctness!)
  timestamp(&start);
  my_sparse_CSR(&per_mat_csr, vec, mysol);
  timestamp(&now);
  printf("Time taken by my own sparse matrix CSR - vector product: %ld ms\n", diff_milli(&start, &now));
  if (check_result(refsol, mysol, size) == 1)
      printf("Result own spare CSR implementation is ok!\n");
  else
      printf("Result own spare CSR implementation is wrong!\n");
  // Your own sparse CSC implementation
  //
  //printf("\nMy Own Spare CSC computation\n----------------\n");

  CSC per_mat_csc = convert_dense_to_CSC(size, mat);
  // Compare times (and computation correctness!)
  timestamp(&start);
  my_sparse_CSC(&per_mat_csc, vec, mysol);
  timestamp(&now);
  printf("Time taken by my own sparse matrix CSC - vector product: %ld ms\n", diff_milli(&start, &now));
  if (check_result(refsol, mysol, size) == 1)
      printf("Result own spare CSC implementation is ok!\n");
  else
      printf("Result own spare CSC implementation is wrong!\n");
  // Free resources
  free(mat);
  free(vec);
  free(refsol);
  free(mysol);
  free_COO(&per_mat_coo);
  free_CSR(&per_mat_csr);
  free_CSC(&per_mat_csc);
  return 0;
}
