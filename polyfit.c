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
#include <math.h>

/*============================================================================*/
/* PRIVATE FUNCTION DECLARATIONS                                             */
/*============================================================================*/

static polyfit_error_t gaussian_elimination(float** A, float* B, float* x,
                                            int32_t n);
static polyfit_error_t allocate_matrix(float*** matrix, int32_t rows,
                                       int32_t cols);
static void free_matrix(float** matrix, int32_t rows);
static polyfit_error_t validate_input_arrays(const float* x, const float* y,
                                             int32_t num_points);

/*============================================================================*/
/* PUBLIC FUNCTION IMPLEMENTATIONS                                           */
/*============================================================================*/

Polynomial* polyfit_init(int32_t degree) {
  if (degree < 0 || degree > POLYFIT_MAX_DEGREE) {
    return NULL;
  }

  Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
  if (poly == NULL) {
    return NULL;
  }

  poly->coefficients = (float*)calloc(degree + 1, sizeof(float));
  if (poly->coefficients == NULL) {
    free(poly);
    return NULL;
  }

  poly->degree = degree;
  poly->is_valid = true;

  return poly;
}

void polyfit_free(Polynomial* poly) {
  if (poly != NULL) {
    free(poly->coefficients);
    poly->coefficients = NULL;
    poly->degree = -1;
    poly->is_valid = false;
    free(poly);
  }
}

polyfit_error_t polyfit_least_squares(const float* x, const float* y,
                                      int32_t num_points, int32_t degree,
                                      Polynomial* result_poly) {
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
  float** A = NULL;
  float* B = NULL;

  error = allocate_matrix(&A, degree + 1, degree + 1);
  if (error != POLYFIT_SUCCESS) {
    return error;
  }

  B = (float*)calloc(degree + 1, sizeof(float));
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

polyfit_error_t polyfit_evaluate(const Polynomial* poly, float x,
                                 float* result) {
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

  return POLYFIT_SUCCESS;
}

polyfit_error_t polyfit_get_max_coefficient_magnitude(const Polynomial* poly,
                                                      float* max_magnitude) {
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

bool polyfit_is_valid(const Polynomial* poly) {
  return (poly != NULL && poly->coefficients != NULL && poly->degree >= 0 &&
          poly->degree <= POLYFIT_MAX_DEGREE && poly->is_valid);
}

const char* polyfit_error_string(polyfit_error_t error) {
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
/* CONVENIENCE FUNCTION IMPLEMENTATIONS                                      */
/*============================================================================*/

Polynomial* polyfit(const float* x, const float* y, int32_t num_points,
                    int32_t degree, polyfit_error_t* error) {
  polyfit_error_t local_error;

  // Initialize polynomial
  Polynomial* poly = polyfit_init(degree);
  if (poly == NULL) {
    local_error = POLYFIT_ERROR_MEMORY_ALLOC;
    if (error != NULL) {
      *error = local_error;
    }
    return NULL;
  }

  // Fit the polynomial
  local_error = polyfit_least_squares(x, y, num_points, degree, poly);
  if (local_error != POLYFIT_SUCCESS) {
    polyfit_free(poly);
    if (error != NULL) {
      *error = local_error;
    }
    return NULL;
  }

  if (error != NULL) {
    *error = POLYFIT_SUCCESS;
  }

  return poly;
}

polyfit_error_t polyfit_eval_at(const float* x, const float* y,
                                int32_t num_points, int32_t degree,
                                float eval_x, float* result) {
  if (result == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  // Fit the polynomial
  polyfit_error_t error;
  Polynomial* poly = polyfit(x, y, num_points, degree, &error);
  if (poly == NULL) {
    return error;
  }

  // Evaluate at the specified point
  error = polyfit_evaluate(poly, eval_x, result);

  // Clean up
  polyfit_free(poly);

  return error;
}

polyfit_error_t polyfit_get_coefficients(const Polynomial* poly, float* coeffs,
                                         int32_t size) {
  if (poly == NULL || coeffs == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (!polyfit_is_valid(poly)) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  if (size < poly->degree + 1) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  // Copy coefficients to user array
  for (int32_t i = 0; i <= poly->degree; i++) {
    coeffs[i] = poly->coefficients[i];
  }

  return POLYFIT_SUCCESS;
}

/*============================================================================*/
/* FIT QUALITY AND AUTO-DEGREE IMPLEMENTATIONS                               */
/*============================================================================*/

polyfit_error_t polyfit_compute_residuals(const Polynomial* poly,
                                          const float* x, const float* y,
                                          int32_t num_points,
                                          float* residuals) {
  if (poly == NULL || x == NULL || y == NULL || residuals == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (!polyfit_is_valid(poly)) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  if (num_points <= 0) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  for (int32_t i = 0; i < num_points; i++) {
    float y_hat;
    polyfit_error_t err = polyfit_evaluate(poly, x[i], &y_hat);
    if (err != POLYFIT_SUCCESS) {
      return err;
    }
    residuals[i] = y_hat - y[i];
  }

  return POLYFIT_SUCCESS;
}

polyfit_error_t polyfit_r_squared(const Polynomial* poly, const float* x,
                                  const float* y, int32_t num_points,
                                  float* r_squared) {
  if (poly == NULL || x == NULL || y == NULL || r_squared == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (!polyfit_is_valid(poly)) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  if (num_points <= 0) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  // Compute mean of y
  float y_mean = 0.0f;
  for (int32_t i = 0; i < num_points; i++) {
    y_mean += y[i];
  }
  y_mean /= (float)num_points;

  // Compute total and residual sums of squares
  float ss_tot = 0.0f;
  float ss_res = 0.0f;

  for (int32_t i = 0; i < num_points; i++) {
    float y_hat;
    polyfit_error_t err = polyfit_evaluate(poly, x[i], &y_hat);
    if (err != POLYFIT_SUCCESS) {
      return err;
    }
    float diff_res = y_hat - y[i];
    float diff_tot = y[i] - y_mean;
    ss_res += diff_res * diff_res;
    ss_tot += diff_tot * diff_tot;
  }

  if (ss_tot < 1e-20f) {
    *r_squared = (ss_res < 1e-20f) ? 1.0f : 0.0f;
    return POLYFIT_SUCCESS;
  }

  *r_squared = 1.0f - (ss_res / ss_tot);

  return POLYFIT_SUCCESS;
}

Polynomial* polyfit_best_degree(const float* x, const float* y,
                                int32_t num_points, int32_t max_degree,
                                int32_t* best_degree,
                                polyfit_error_t* error) {
  if (x == NULL || y == NULL || best_degree == NULL) {
    if (error != NULL) {
      *error = POLYFIT_ERROR_NULL_POINTER;
    }
    return NULL;
  }

  if (max_degree < 1 || max_degree > POLYFIT_MAX_DEGREE) {
    if (error != NULL) {
      *error = POLYFIT_ERROR_INVALID_DEGREE;
    }
    return NULL;
  }

  if (num_points <= max_degree) {
    if (error != NULL) {
      *error = POLYFIT_ERROR_INSUFFICIENT_POINTS;
    }
    return NULL;
  }

  Polynomial* best_poly = NULL;
  float best_bic = 0.0f;
  float fn = (float)num_points;

  for (int32_t d = 1; d <= max_degree; d++) {
    polyfit_error_t local_err;
    Polynomial* poly = polyfit(x, y, num_points, d, &local_err);
    if (poly == NULL) {
      continue;
    }

    // Compute residual sum of squares
    float ss_res = 0.0f;
    for (int32_t i = 0; i < num_points; i++) {
      float y_hat;
      if (polyfit_evaluate(poly, x[i], &y_hat) == POLYFIT_SUCCESS) {
        float diff = y_hat - y[i];
        ss_res += diff * diff;
      }
    }

    // BIC = n * ln(RSS/n) + (d+1) * ln(n)
    float bic;
    if (ss_res < 1e-30f) {
      bic = -1e30f;  // Near-perfect fit
    } else {
      bic = fn * logf(ss_res / fn) + (float)(d + 1) * logf(fn);
    }

    if (best_poly == NULL || bic < best_bic) {
      best_bic = bic;
      *best_degree = d;
      polyfit_free(best_poly);
      best_poly = poly;
    } else {
      polyfit_free(poly);
    }
  }

  if (best_poly == NULL) {
    if (error != NULL) {
      *error = POLYFIT_ERROR_SINGULAR_MATRIX;
    }
    return NULL;
  }

  if (error != NULL) {
    *error = POLYFIT_SUCCESS;
  }

  return best_poly;
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

static polyfit_error_t gaussian_elimination(float** A, float* B, float* x,
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
      float* temp_row = A[i];
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

static polyfit_error_t allocate_matrix(float*** matrix, int32_t rows,
                                       int32_t cols) {
  if (matrix == NULL || rows <= 0 || cols <= 0) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  *matrix = (float**)malloc(rows * sizeof(float*));
  if (*matrix == NULL) {
    return POLYFIT_ERROR_MEMORY_ALLOC;
  }

  for (int32_t i = 0; i < rows; i++) {
    (*matrix)[i] = (float*)calloc(cols, sizeof(float));
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

static void free_matrix(float** matrix, int32_t rows) {
  if (matrix != NULL) {
    for (int32_t i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

static polyfit_error_t validate_input_arrays(const float* x, const float* y,
                                             int32_t num_points) {
  if (x == NULL || y == NULL) {
    return POLYFIT_ERROR_NULL_POINTER;
  }

  if (num_points <= 0) {
    return POLYFIT_ERROR_INVALID_INPUT;
  }

  // Check for NaN or infinite values
  for (int32_t i = 0; i < num_points; i++) {
    if (x[i] - x[i] != 0.0f || y[i] - y[i] != 0.0f) {  // NaN and Inf check
      return POLYFIT_ERROR_INVALID_INPUT;
    }
  }

  return POLYFIT_SUCCESS;
}

