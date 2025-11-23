/**
 ******************************************************************************
 * @file    polyfit.h
 * @brief   Header file for polynomial fitting and evaluation library
 * @version 1.0
 * @date    2025
 ******************************************************************************
 * @attention
 *
 * This library provides polynomial fitting using least squares regression
 * and polynomial evaluation capabilities for embedded systems.
 *
 ******************************************************************************
 */

#ifndef POLYFIT_H_
#define POLYFIT_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* CONSTANTS AND CONFIGURATION                                               */
/*============================================================================*/

/** @brief Default absolute threshold for near-zero values */
#define POLYFIT_ABSOLUTE_THRESHOLD (1e-6f)

/** @brief Default relative threshold for near-zero values */
#define POLYFIT_RELATIVE_THRESHOLD (1e-6f)

/** @brief Maximum supported polynomial degree */
#define POLYFIT_MAX_DEGREE (10)

/*============================================================================*/
/* TYPE DEFINITIONS                                                           */
/*============================================================================*/

/**
 * @brief Error codes for polynomial operations
 */
typedef enum {
  POLYFIT_SUCCESS = 0,               /**< Operation successful */
  POLYFIT_ERROR_NULL_POINTER,        /**< Null pointer passed */
  POLYFIT_ERROR_INVALID_DEGREE,      /**< Invalid polynomial degree */
  POLYFIT_ERROR_MEMORY_ALLOC,        /**< Memory allocation failed */
  POLYFIT_ERROR_SINGULAR_MATRIX,     /**< Matrix is singular */
  POLYFIT_ERROR_INSUFFICIENT_POINTS, /**< Not enough data points */
  POLYFIT_ERROR_INVALID_INPUT        /**< Invalid input parameters */
} polyfit_error_t;

/**
 * @brief Structure to represent a polynomial
 */
typedef struct {
  float* coefficients; /**< Array of polynomial coefficients */
  int32_t degree;      /**< Degree of the polynomial */
  bool is_valid;       /**< Flag indicating if polynomial is valid */
} Polynomial;

/**
 * @brief Configuration structure for polynomial fitting
 */
typedef struct {
  float absolute_threshold; /**< Absolute threshold for near-zero values */
  float relative_threshold; /**< Relative threshold for near-zero values */
  bool enable_pivot_check; /**< Enable pivot checking in Gaussian elimination */
} polyfit_config_t;

/*============================================================================*/
/* FUNCTION DECLARATIONS                                                      */
/*============================================================================*/

/**
 * @brief Initialize a polynomial structure
 * @param degree Degree of the polynomial (must be >= 0)
 * @return Pointer to initialized Polynomial structure, or NULL on failure
 * @note Caller is responsible for freeing the returned polynomial
 */
Polynomial* polyfit_init(int32_t degree);

/**
 * @brief Free memory allocated for a polynomial
 * @param poly Pointer to the Polynomial structure to be freed
 * @note Sets the polynomial to invalid state after freeing
 */
void polyfit_free(Polynomial* poly);

/**
 * @brief Perform least squares polynomial regression
 * @param x Array of x values (must not be NULL)
 * @param y Array of corresponding y values (must not be NULL)
 * @param num_points Number of data points (must be > degree)
 * @param degree Degree of the polynomial (must be >= 0)
 * @param result_poly Pointer to store the resulting polynomial (must not be
 * NULL)
 * @return Error code indicating success or failure
 */
polyfit_error_t polyfit_least_squares(const float* x, const float* y,
                                      int32_t num_points, int32_t degree,
                                      Polynomial* result_poly);

/**
 * @brief Evaluate a polynomial at a given x value
 * @param poly Pointer to the Polynomial structure (must not be NULL)
 * @param x Value at which to evaluate the polynomial
 * @param result Pointer to store the evaluation result (must not be NULL)
 * @return Error code indicating success or failure
 */
polyfit_error_t polyfit_evaluate(const Polynomial* poly, float x,
                                 float* result);

/**
 * @brief Get the maximum absolute magnitude among polynomial coefficients
 * @param poly Pointer to the Polynomial structure (must not be NULL)
 * @param max_magnitude Pointer to store the maximum magnitude (must not be
 * NULL)
 * @return Error code indicating success or failure
 */
polyfit_error_t polyfit_get_max_coefficient_magnitude(const Polynomial* poly,
                                                      float* max_magnitude);

/**
 * @brief Validate a polynomial structure
 * @param poly Pointer to the Polynomial structure
 * @return true if polynomial is valid, false otherwise
 */
bool polyfit_is_valid(const Polynomial* poly);

/**
 * @brief Get string representation of error code
 * @param error Error code
 * @return String representation of the error
 */
const char* polyfit_error_string(polyfit_error_t error);

/*============================================================================*/
/* CONVENIENCE FUNCTIONS                                                      */
/*============================================================================*/

/**
 * @brief Simplified polynomial fitting - allocates and fits in one call
 * @param x Array of x values (must not be NULL)
 * @param y Array of corresponding y values (must not be NULL)
 * @param num_points Number of data points (must be > degree)
 * @param degree Degree of the polynomial (must be >= 0)
 * @param error Optional pointer to store error code (can be NULL)
 * @return Pointer to fitted Polynomial structure, or NULL on failure
 * @note Caller is responsible for freeing with polyfit_free()
 *
 * @example
 * float x[] = {1, 2, 3, 4, 5};
 * float y[] = {2, 4, 6, 8, 10};
 * Polynomial *poly = polyfit(x, y, 5, 1, NULL);
 * if (poly) {
 *     float result;
 *     polyfit_evaluate(poly, 6.0f, &result);
 *     polyfit_free(poly);
 * }
 */
Polynomial* polyfit(const float* x, const float* y, int32_t num_points,
                    int32_t degree, polyfit_error_t* error);

/**
 * @brief Fit polynomial and evaluate at a single point
 * @param x Array of x values for fitting (must not be NULL)
 * @param y Array of y values for fitting (must not be NULL)
 * @param num_points Number of data points (must be > degree)
 * @param degree Degree of the polynomial (must be >= 0)
 * @param eval_x Point at which to evaluate the fitted polynomial
 * @param result Pointer to store the evaluation result (must not be NULL)
 * @return Error code indicating success or failure
 * @note This is a convenience function that doesn't require manual memory
 * management
 *
 * @example
 * float x[] = {1, 2, 3, 4, 5};
 * float y[] = {2, 4, 6, 8, 10};
 * float result;
 * if (polyfit_eval_at(x, y, 5, 1, 6.0f, &result) == POLYFIT_SUCCESS) {
 *     printf("f(6) = %f\n", result);
 * }
 */
polyfit_error_t polyfit_eval_at(const float* x, const float* y,
                                int32_t num_points, int32_t degree,
                                float eval_x, float* result);

/**
 * @brief Get polynomial coefficients as an array
 * @param poly Pointer to the Polynomial structure (must not be NULL)
 * @param coeffs Array to store coefficients (must not be NULL, size >=
 * degree+1)
 * @param size Size of the coeffs array
 * @return Error code indicating success or failure
 * @note Coefficients are in ascending order: coeffs[0] + coeffs[1]*x +
 * coeffs[2]*x^2 + ...
 *
 * @example
 * float coeffs[3];
 * if (polyfit_get_coefficients(poly, coeffs, 3) == POLYFIT_SUCCESS) {
 *     printf("Polynomial: %f + %f*x + %f*x^2\n", coeffs[0], coeffs[1],
 * coeffs[2]);
 * }
 */
polyfit_error_t polyfit_get_coefficients(const Polynomial* poly, float* coeffs,
                                         int32_t size);

/*============================================================================*/
/* UTILITY FUNCTIONS                                                          */
/*============================================================================*/

/**
 * @brief Calculate power of a base to an integer exponent
 * @param base The base value
 * @param exponent The exponent value
 * @return Result of base raised to the power of exponent
 * @note Handles special cases like 0^0 and negative exponents
 */
float polyfit_pow(float base, int32_t exponent);

/**
 * @brief Calculate absolute value of a floating-point number
 * @param x The input floating-point number
 * @return Absolute value of x
 */
float polyfit_fabs(float x);

/**
 * @brief Check if a floating-point number is nearly zero
 * @param value The value to check
 * @param threshold The threshold for comparison
 * @return true if value is nearly zero, false otherwise
 */
bool polyfit_is_nearly_zero(float value, float threshold);

#ifdef __cplusplus
}
#endif

#endif /* POLYFIT_H_ */