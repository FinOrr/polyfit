/**
 ******************************************************************************
 * @file polyFit.c
 * @brief   Source file for fitting polynomials to datasets.
 ******************************************************************************
 * @attention
 *
 * This file is heavily dependent on dynamic memory allocation, ew.
 * I had to develop this quickly over a day and test on the host machine.
 * Malloc was faster to develop with, but I know it is not ideal for embedded.
 * Will refactor to static alloc when I have the time ಥ_ಥ
 *
 ******************************************************************************
 */

#include "polyFit.h"

/**
 * @brief Initialise a polynomial and return a pointer to it.
 *
 * @param degree Degree of the polynomial.
 * @return Pointer to the Initialised polynomial.
 */
Polynomial *initPolynomial(int32_t degree) {
  Polynomial *poly = (Polynomial *)malloc(sizeof(Polynomial));

  // Check if memory allocation was successful
  if (poly == NULL) {
    // Error: Memory allocation failed for polynomial structure.
    exit(EXIT_FAILURE);  // or handle the error in a way suitable for your
                         // application
  }

  poly->coefficients = (float *)malloc((degree + 1) * sizeof(float));

  // Check if memory allocation was successful
  if (poly->coefficients == NULL) {
    // Error: Memory allocation failed for polynomial coefficients
    free(poly);  // free the previously allocated memory for the structure
    exit(EXIT_FAILURE);  // or handle the error in a way suitable for your
                         // application
  }

  // Init coefficients to zero
  for (int32_t i = 0; i <= degree; ++i) {
    poly->coefficients[i] = 0.0;
  }

  poly->degree = degree;
  return (poly);
}

/**
 * @brief Free memory allocated for a polynomial.
 *
 * @param poly Pointer to the polynomial to be freed.
 */
void freePolynomial(Polynomial *poly) { free(poly->coefficients); }

/**
 * @brief Perform Gaussian elimination to solve a system of linear equations.
 *
 * @param A Coefficient matrix.
 * @param B Right-hand side vector.
 * @param x Solution vector.
 * @param n Size of the system.
 */
void gaussianElimination(float **A, float *B, float *x, int32_t n) {
  for (int32_t i = 0; i < n; ++i) {
    // Pivot
    int32_t max = i;
    for (int32_t j = i + 1; j < n; ++j) {
      if (tinyFabs(A[j][i]) > tinyFabs(A[max][i])) {
        max = j;
      }
    }

    // Swap rows i and max
    float *temp = A[i];
    A[i] = A[max];
    A[max] = temp;
    float t = B[i];
    B[i] = B[max];
    B[max] = t;

    // Forward elimination
    for (int32_t j = i + 1; j < n; ++j) {
      float f = A[j][i] / A[i][i];
      for (int32_t k = i; k < n; ++k) {
        A[j][k] -= A[i][k] * f;
      }
      B[j] -= B[i] * f;
    }
  }

  // Backward substitution
  for (int32_t i = n - 1; i >= 0; --i) {
    for (int32_t j = i + 1; j < n; ++j) {
      B[i] -= A[i][j] * x[j];
    }
    x[i] = B[i] / A[i][i];
  }
}

/**
 * @brief Perform least squares polynomial regression.
 *
 * @param x Array of x values.
 * @param y Array of corresponding y values.
 * @param numPoints Number of data points.
 * @param degree Degree of the polynomial regression.
 * @param resultPoly Pointer to store the result polynomial.
 */
void leastSquaresPolynomialRegression(const float *x, const float *y,
                                      int32_t numPoints, int32_t degree,
                                      Polynomial *resultPoly) {
  // Allocate matrices A and B
  float **A = (float **)malloc((degree + 1) * sizeof(float *));
  for (int32_t i = 0; i <= degree; ++i) {
    A[i] = (float *)malloc((degree + 1) * sizeof(float));
  }
  float *B = (float *)malloc((degree + 1) * sizeof(float));

  // Initialise matrices A and B
  for (int32_t i = 0; i <= degree; ++i) {
    B[i] = 0.0;
    for (int32_t j = 0; j <= degree; ++j) {
      A[i][j] = 0.0;
      for (int32_t k = 0; k < numPoints; ++k) {
        A[i][j] += tinyPow(x[k], i + j);
      }
    }
    for (int32_t k = 0; k < numPoints; ++k) {
      B[i] += y[k] * tinyPow(x[k], i);
    }
  }

  // Solve the system of linear equations (Ax = B) for coefficients
  gaussianElimination(A, B, resultPoly->coefficients, degree + 1);

  // Free allocated memory for matrices
  for (int32_t i = 0; i <= degree; ++i) {
    free(A[i]);
  }
  free(A);
  free(B);
}

/**
 * @brief Evaluate the polynomial at a given x value.
 *
 * @param poly Pointer to the polynomial.
 * @param x Value at which to evaluate the polynomial.
 * @return Result of the polynomial evaluation.
 */
float evaluatePolynomial(const Polynomial *poly, float x) {
  if (poly == NULL || poly->coefficients == NULL) {
    return 0.0f;
  }

  float result = 0.0;

  for (int32_t i = 0; i <= poly->degree; ++i) {
    result += poly->coefficients[i] * tinyPow(x, i);
  }

  // Apply a combination of relative and absolute thresholds
  // Absolute threshold to consider a value negligible
  float absoluteThreshold = 1e-6;
  // Relative threshold relative to coefficient magnitudes
  float relativeThreshold = 1e-6;

  // If the result is close to zero, set it to zero
  if (tinyFabs(result) <
          relativeThreshold *
              getMaxCoefficientMagnitude(poly->coefficients, poly->degree) ||
      tinyFabs(result) < absoluteThreshold) {
    result = 0.0f;
  }

  return (result);
}

/**
 * @brief Get the maximum absolute magnitude among the polynomial coefficients.
 *
 * @param coefficients Array of polynomial coefficients.
 * @param degree Degree of the polynomial.
 * @return Maximum absolute magnitude among the coefficients.
 */
float getMaxCoefficientMagnitude(const float *coefficients, int32_t degree) {
  if (coefficients == NULL) {
    return 0.0f;
  }

  float maxMagnitude = 0.0;

  for (int32_t i = 0; i <= degree; ++i) {
    float magnitude = tinyFabs(coefficients[i]);
    if (magnitude > maxMagnitude) {
      maxMagnitude = magnitude;
    }
  }

  return (maxMagnitude);
}

/**
 * @brief Function to calculate the power of a base to an exponent.
 * @brief This is to avoid having to link the MASSIVE math.h library.
 * @param base The base value (float).
 * @param exponent The exponent value (signed 32-bit integer).
 * @return Result of base raised to the power of exponent.
 */
float tinyPow(float base, int32_t exponent) {
  // Check for special cases
  if (base == 0.0f) {
    if (exponent == 0) {
      return 1.0f;  // 0^0 is considered 1 by convention
    } else if (exponent < 0) {
      // Handling 0 ^ negative_exponent is undefined, return an error value or
      // NaN as needed
      return (0.0f);  // Adjust this according to your specific requirements
    }
  }

  // Initialise result
  float result = 1.0f;

  // Determine the positive or negative exponent
  int32_t absExponent = exponent > 0 ? exponent : -exponent;

  // Calculate power using binary exponentiation for efficiency
  while (absExponent > 0) {
    if (absExponent % 2 == 1) {
      result *= base;
    }
    base *= base;
    absExponent /= 2;
  }

  // Adjust result for negative exponent
  if (exponent < 0) {
    result = 1.0f / result;
  }

  return (result);
}

/**
 * @brief Calculate the absolute value of a floating-point number.
 * @param x The input floating-point number.
 * @return Absolute value of x.
 */
float tinyFabs(float x) {
  // Handle NaN (Not-a-Number)
  if (!(x == x)) {
    // Return NaN if x is NaN
    return (x);
  }

  // Handle negative zero
  if (x == 0.0f && *((uint32_t *)&x) & 0x80000000) {
    // Return positive zero if x is negative zero
    return (0.0f);
  }

  return ((x < 0.0f) ? -x : x);
}
