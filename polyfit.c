/**
 ******************************************************************************
 * @file    polyfit.c
 * @brief   Implementation of polynomial fitting and evaluation library
 * @version 1.0
 * @date    2025
 ******************************************************************************
 * @attention
 *
 * This implementation provides robust polynomial fitting using least squares
 * regression with proper error handling and memory management.
 *
 ******************************************************************************
 */

#include "polyfit.h"

/*============================================================================*/
/* PRIVATE FUNCTION DECLARATIONS                                             */
/*============================================================================*/

static polyfit_error_t gaussian_elimination(float **A, float *B, float *x,
                                            int32_t n);
static polyfit_error_t allocate_matrix(float ***matrix, int32_t rows,
                                       int32_t cols);
static void free_matrix(float **matrix, int32_t rows);
static polyfit_error_t validate_input_arrays(const float *x, const float *y,
                                             int32_t num_points);
static bool is_matrix_singular(float **A, int32_t n);

/*============================================================================*/
/* GLOBAL VARIABLES                                                           */
/*============================================================================*/

static const polyfit_config_t default_config = {
    .absolute_threshold = POLYFIT_ABSOLUTE_THRESHOLD,
    .relative_threshold = POLYFIT_RELATIVE_THRESHOLD,
    .enable_pivot_check = true};

/*============================================================================*/
/* PUBLIC FUNCTION IMPLEMENTATIONS                                           */
/*============================================================================*/

Polynomial *polyfit_init(int32_t degree) {
  if (degree < 0 || degree > POLYFIT_MAX_DEGREE) {
    return NULL;
  }

  Polynomial *poly = (Polynomial *)malloc(sizeof(Polynomial));
  if (poly == NULL) {
    return NULL;
  }

  poly->coefficients = (float *)calloc(degree + 1, sizeof(float));
  if (poly->coefficients == NULL) {
    free(poly);
    return NULL;
  }

  poly->degree = degree;
  poly->is_valid = true;

  return poly;
}

void polyfit_free(Polynomial *poly) {
  if (poly != NULL) {
    free(poly->coefficients);
    poly->coefficients = NULL;
    poly->degree = -1;
    poly->is_valid = false;
    free(poly);
  }
}

polyfit_error_t polyfit_least_squares(const float *x, const float *y,
                                      int32_t num_points, int32_t degree,
                                      Polynomial *result_poly) {
  // Input validation
  if (x == NULL || y == NULL || result_poly == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (degree < 0 || degree > POLYFIT_MAX_DEGREE) {
    return POLYFIT_ERROR_INVALID_DEGREE;
  }

  if (num_points <= degree) {
    return POLYFIT_ERROR_INSUFFICIENT_POINTS;
  }

  polyfit_error_t error = validate_input_arrays(x, y, num_points);
  if (error != POLYFIT_SUCCESS) {
    return error;
  }

  // Allocate matrices
  float **A = NULL;
  float *B = NULL;

  error = allocate_matrix(&A, degree + 1, degree + 1);
  if (error != POLYFIT_SUCCESS) {
    return error;
  }

  B = (float *)calloc(degree + 1, sizeof(float));
  if (B == NULL) {
    free_matrix(A, degree + 1);
    return POLYFIT_ERROR_MEMORY_ALLOC;
  }

  // Build normal equations (A^T * A * coeffs = A^T * y)
  for (int32_t i = 0; i <= degree; i++) {
    for (int32_t j = 0; j <= degree; j++) {
      A[i][j] = 0.0f;
      for (int32_t k = 0; k < num_points; k++) {
        A[i][j] += polyfit_pow(x[k], i + j);
      }
    }

    B[i] = 0.0f;
    for (int32_t k = 0; k < num_points; k++) {
      B[i] += y[k] * polyfit_pow(x[k], i);
    }
  }

  // Check for singular matrix
  if (is_matrix_singular(A, degree + 1)) {
    free_matrix(A, degree + 1);
    free(B);
    return POLYFIT_ERROR_SINGULAR_MATRIX;
  }

  // Solve the system
  error = gaussian_elimination(A, B, result_poly->coefficients, degree + 1);

  // Cleanup
  free_matrix(A, degree + 1);
  free(B);

  if (error == POLYFIT_SUCCESS) {
    result_poly->degree = degree;
    result_poly->is_valid = true;
  }

  return error;
}

polyfit_error_t polyfit_evaluate(const Polynomial *poly, float x,
                                 float *result) {
  if (poly == NULL || result == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (!polyfit_is_valid(poly)) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  *result = 0.0f;

  // Use Horner's method for efficient evaluation
  for (int32_t i = poly->degree; i >= 0; i--) {
    *result = *result * x + poly->coefficients[i];
  }

  // Apply thresholding
  float max_magnitude;
  polyfit_get_max_coefficient_magnitude(poly, &max_magnitude);

  if (polyfit_is_nearly_zero(*result, default_config.absolute_threshold) ||
      polyfit_is_nearly_zero(
          *result, default_config.relative_threshold * max_magnitude)) {
    *result = 0.0f;
  }

  return POLYFIT_SUCCESS;
}

polyfit_error_t polyfit_get_max_coefficient_magnitude(const Polynomial *poly,
                                                      float *max_magnitude) {
  if (poly == NULL || max_magnitude == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (!polyfit_is_valid(poly)) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  *max_magnitude = 0.0f;

  for (int32_t i = 0; i <= poly->degree; i++) {
    float magnitude = polyfit_fabs(poly->coefficients[i]);
    if (magnitude > *max_magnitude) {
      *max_magnitude = magnitude;
    }
  }

  return POLYFIT_SUCCESS;
}

bool polyfit_is_valid(const Polynomial *poly) {
  return (poly != NULL && poly->coefficients != NULL && poly->degree >= 0 &&
          poly->degree <= POLYFIT_MAX_DEGREE && poly->is_valid);
}

const char *polyfit_error_string(polyfit_error_t error) {
  switch (error) {
    case POLYFIT_SUCCESS:
      return "Success";
    case POLYFIT_ERROR_NULL_POINTER:
      return "Null pointer provided";
    case POLYFIT_ERROR_INVALID_DEGREE:
      return "Invalid polynomial degree";
    case POLYFIT_ERROR_MEMORY_ALLOC:
      return "Memory allocation failed";
    case POLYFIT_ERROR_SINGULAR_MATRIX:
      return "Matrix is singular";
    case POLYFIT_ERROR_INSUFFICIENT_POINTS:
      return "Insufficient data points";
    case POLYFIT_ERROR_INVALID_INPUT:
      return "Invalid input parameters";
    default:
      return "Unknown error";
  }
}

/*============================================================================*/
/* UTILITY FUNCTION IMPLEMENTATIONS                                          */
/*============================================================================*/

float polyfit_pow(float base, int32_t exponent) {
  // Handle special cases
  if (exponent == 0) {
    return 1.0f;
  }

  if (base == 0.0f) {
    return (exponent > 0) ? 0.0f : 1.0f;  // 0^0 = 1 by convention
  }

  if (base == 1.0f) {
    return 1.0f;
  }

  if (base == -1.0f) {
    return (exponent % 2 == 0) ? 1.0f : -1.0f;
  }

  // Use binary exponentiation
  float result = 1.0f;
  float current_base = base;
  int32_t abs_exponent = (exponent < 0) ? -exponent : exponent;

  while (abs_exponent > 0) {
    if (abs_exponent & 1) {
      result *= current_base;
    }
    current_base *= current_base;
    abs_exponent >>= 1;
  }

  return (exponent < 0) ? (1.0f / result) : result;
}

float polyfit_fabs(float x) {
  // Handle NaN
  if (x != x) {
    return x;
  }

  // Use bit manipulation for efficiency
  union {
    float f;
    uint32_t i;
  } u;

  u.f = x;
  u.i &= 0x7FFFFFFF;  // Clear sign bit

  return u.f;
}

bool polyfit_is_nearly_zero(float value, float threshold) {
  return polyfit_fabs(value) < threshold;
}

/*============================================================================*/
/* PRIVATE FUNCTION IMPLEMENTATIONS                                          */
/*============================================================================*/

static polyfit_error_t gaussian_elimination(float **A, float *B, float *x,
                                            int32_t n) {
  if (A == NULL || B == NULL || x == NULL || n <= 0) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  const float pivot_threshold = 1e-12f;

  // Forward elimination with partial pivoting
  for (int32_t i = 0; i < n; i++) {
    // Find pivot
    int32_t max_row = i;
    for (int32_t k = i + 1; k < n; k++) {
      if (polyfit_fabs(A[k][i]) > polyfit_fabs(A[max_row][i])) {
        max_row = k;
      }
    }

    // Check for singular matrix
    if (polyfit_fabs(A[max_row][i]) < pivot_threshold) {
      return POLYFIT_ERROR_SINGULAR_MATRIX;
    }

    // Swap rows
    if (max_row != i) {
      float *temp_row = A[i];
      A[i] = A[max_row];
      A[max_row] = temp_row;

      float temp_b = B[i];
      B[i] = B[max_row];
      B[max_row] = temp_b;
    }

    // Eliminate column
    for (int32_t k = i + 1; k < n; k++) {
      float factor = A[k][i] / A[i][i];
      for (int32_t j = i; j < n; j++) {
        A[k][j] -= factor * A[i][j];
      }
      B[k] -= factor * B[i];
    }
  }

  // Back substitution
  for (int32_t i = n - 1; i >= 0; i--) {
    x[i] = B[i];
    for (int32_t j = i + 1; j < n; j++) {
      x[i] -= A[i][j] * x[j];
    }
    x[i] /= A[i][i];
  }

  return POLYFIT_SUCCESS;
}

static polyfit_error_t allocate_matrix(float ***matrix, int32_t rows,
                                       int32_t cols) {
  if (matrix == NULL || rows <= 0 || cols <= 0) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  *matrix = (float **)malloc(rows * sizeof(float *));
  if (*matrix == NULL) {
    return POLYFIT_ERROR_MEMORY_ALLOC;
  }

  for (int32_t i = 0; i < rows; i++) {
    (*matrix)[i] = (float *)calloc(cols, sizeof(float));
    if ((*matrix)[i] == NULL) {
      // Clean up previously allocated rows
      for (int32_t j = 0; j < i; j++) {
        free((*matrix)[j]);
      }
      free(*matrix);
      *matrix = NULL;
      return POLYFIT_ERROR_MEMORY_ALLOC;
    }
  }

  return POLYFIT_SUCCESS;
}

static void free_matrix(float **matrix, int32_t rows) {
  if (matrix != NULL) {
    for (int32_t i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

static polyfit_error_t validate_input_arrays(const float *x, const float *y,
                                             int32_t num_points) {
  if (x == NULL || y == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (num_points <= 0) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  // Check for NaN or infinite values
  for (int32_t i = 0; i < num_points; i++) {
    if (x[i] != x[i] || y[i] != y[i]) {  // NaN check
      return POLYFIT_ERROR_INVALID_INPUT;
    }
  }

  return POLYFIT_SUCCESS;
}

static bool is_matrix_singular(float **A, int32_t n) {
  const float determinant_threshold = 1e-12f;

  // Simple check: if any diagonal element is too small after partial pivoting
  for (int32_t i = 0; i < n; i++) {
    float max_in_column = 0.0f;
    for (int32_t j = i; j < n; j++) {
      float abs_val = polyfit_fabs(A[j][i]);
      if (abs_val > max_in_column) {
        max_in_column = abs_val;
      }
    }

    if (max_in_column < determinant_threshold) {
      return true;
    }
  }

  return false;
}