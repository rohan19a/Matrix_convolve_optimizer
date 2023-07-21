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
void convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  // Check if a_matrix or b_matrix is NULL
  if (a_matrix == NULL || b_matrix == NULL) {
    *output_matrix = NULL; // Set the output_matrix pointer to NULL to indicate failure
    return;
  }

  // Allocate space for the output matrix
  *output_matrix = (matrix_t*)malloc(sizeof(matrix_t));

  // Allocate space for the rows and columns of the output matrix
  (*output_matrix)->rows = a_matrix->rows + b_matrix->rows - 1;
  (*output_matrix)->cols = a_matrix->cols + b_matrix->cols - 1;

  // Allocate space for the data of the output matrix
  (*output_matrix)->data = (int32_t*)malloc(sizeof(int32_t) * (*output_matrix)->rows * (*output_matrix)->cols);

  // Flip matrix b in both dimensions (swap rows and columns)
  matrix_t *flipped_b = (matrix_t*)malloc(sizeof(matrix_t));
  flipped_b->rows = b_matrix->cols;
  flipped_b->cols = b_matrix->rows;
  flipped_b->data = (int32_t*)malloc(sizeof(int32_t) * b_matrix->rows * b_matrix->cols);

  for (int i = 0; i < b_matrix->rows; i++) {
    for (int j = 0; j < b_matrix->cols; j++) {
      flipped_b->data[j * b_matrix->rows + i] = b_matrix->data[i * b_matrix->cols + j];
    }
  }

  // Compute the convolution
  for (int i = 0; i < (*output_matrix)->rows; i++) {
    for (int j = 0; j < (*output_matrix)->cols; j++) {
      int32_t sum = 0;
      for (int m = 0; m < a_matrix->rows; m++) {
        for (int n = 0; n < a_matrix->cols; n++) {
          if (i - m >= 0 && i - m < b_matrix->rows && j - n >= 0 && j - n < b_matrix->cols) {
            sum += a_matrix->data[m * a_matrix->cols + n] * flipped_b->data[(i - m) * flipped_b->cols + (j - n)];
          }
        }
      }
      (*output_matrix)->data[i * (*output_matrix)->cols + j] = sum;
    }
  }

  // Free memory for flipped_b
  free(flipped_b->data);
  free(flipped_b);
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
