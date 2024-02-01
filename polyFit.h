/**
 ******************************************************************************
 * @file    polyFit.h
 * @brief   Header file for polynomial extrpolation.
 ******************************************************************************
 */

#ifndef INC_POLYFIT_H_
#define INC_POLYFIT_H_

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Structure to hold the polynomial coefficients
 */
typedef struct {
  float *coefficients; /**< Array of coefficients */
  int32_t degree;      /**< Degree of the polynomial */
} Polynomial;

/**
 * @brief Function to initialise a polynomial
 * @param degree Degree of the polynomial
 * @return Pointer to initialised Polynomial structure
 */
Polynomial *initPolynomial(int32_t degree);

/**
 * @brief Function to free memory allocated for a polynomial
 * @param poly Pointer to the Polynomial structure to be freed
 */
void freePolynomial(Polynomial *poly);

/**
 * @brief Function to perform Gaussian elimination
 * @param A Matrix of coefficients
 * @param B Array of constants
 * @param x Array to store the solution
 * @param n Size of the system (number of equations)
 */
void gaussianElimination(float **A, float *B, float *x, int32_t n);

/**
 * @brief Function to perform least squares polynomial regression
 * @param x Array of x values
 * @param y Array of corresponding y values
 * @param numPoints Number of data points
 * @param degree Degree of the polynomial
 * @param resultPoly Pointer to store the resulting polynomial
 */
void leastSquaresPolynomialRegression(const float *x, const float *y,
                                      int32_t numPoints, int32_t degree,
                                      Polynomial *resultPoly);

/**
 * @brief Function to evaluate the polynomial at a given x value
 * @param poly Pointer to the Polynomial structure
 * @param x Value at which the polynomial is evaluated
 * @return Result of the polynomial evaluation
 */
float evaluatePolynomial(const Polynomial *poly, float x);

/**
 * @brief Get the maximum absolute magnitude among the polynomial coefficients.
 *
 * @param coefficients Array of polynomial coefficients.
 * @param degree Degree of the polynomial.
 * @return Maximum absolute magnitude among the coefficients.
 */
float getMaxCoefficientMagnitude(const float *coefficients, int32_t degree);

#endif /* INC_POLYFIT_H_ */
