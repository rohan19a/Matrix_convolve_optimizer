#include "compute.h"

// Computes the dot product of vec1 and vec2, both of size n
int dot(uint32_t n, int32_t *vec1, int32_t *vec2) {
  // TODO: implement dot product of vec1 and vec2, both of size n
  int sum = 0;

  for (int i = 0; i < n; i++) {
    sum += vec1[i] * vec2[i];
  }

  return sum;
}

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // TODO: convolve matrix a and matrix b, and store the resulting matrix in
  // output_matrix

  //Flip matrix b in both dimensions

  if a_matrix == NULL || b_matrix == NULL {
    return -1;
  }

  *output_matrix = (matrix_t*)malloc(size_of(matrix_t));
  (*output_matrix)->rows = a_matrix->rows;
  (*output_matrix)->cols = a_matrix->cols;
  (*output_matrix)->data = (int32_t*)malloc(sizeof(int32_t) * a_matrix->rows * a_matrix->cols);

  

  return output_matrix;
}

// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  if (read_matrix(get_a_matrix_path(task), &a_matrix))
    return -1;
  if (read_matrix(get_b_matrix_path(task), &b_matrix))
    return -1;

  if (convolve(a_matrix, b_matrix, &output_matrix))
    return -1;

  if (write_matrix(get_output_matrix_path(task), output_matrix))
    return -1;

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
