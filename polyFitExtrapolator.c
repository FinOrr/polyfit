/**
 ******************************************************************************
 * @file cPolyfitExtrapolator.c
 * @brief Implement a least squares polynomial regression to estimate RPM.
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

#include "polyFitExtrapolator.h"

/**
 * @brief Initialise a polynomial and return a pointer to it.
 *
 * @param degree Degree of the polynomial.
 * @return Pointer to the Initialised polynomial.
 */
Polynomial* initPolynomial(uint32_t degree)
{
    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));

    // Check if memory allocation was successful
    if (poly == NULL) {
        // Error: Memory allocation failed for polynomial structure.
        exit(EXIT_FAILURE); // or handle the error in a way suitable for your application
    }

    poly->coefficients = (float*)malloc((degree + 1) * sizeof(float));

    // Check if memory allocation was successful
    if (poly->coefficients == NULL) {
        // Error: Memory allocation failed for polynomial coefficients
        free(poly); // free the previously allocated memory for the structure
        exit(EXIT_FAILURE); // or handle the error in a way suitable for your application
    }

    // Init coefficients to zero
    for (uint32_t i = 0; i <= degree; ++i) {
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
void freePolynomial(Polynomial* poly)
{
    free(poly->coefficients);
}

/**
 * @brief Perform Gaussian elimination to solve a system of linear equations.
 *
 * @param A Coefficient matrix.
 * @param B Right-hand side vector.
 * @param x Solution vector.
 * @param n Size of the system.
 */
void gaussianElimination(float** A, float* B, float* x, int32_t n)
{
    for (int32_t i = 0; i < n; ++i) {
        // Pivot
        int32_t max = i;
        for (int32_t j = i + 1; j < n; ++j) {
            if (fabs(A[j][i]) > fabs(A[max][i])) {
                max = j;
            }
        }

        // Swap rows i and max
        float* temp = A[i];
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
void leastSquaresPolynomialRegression(const float* x, const float* y, int32_t numPoints, int32_t degree,
    Polynomial* resultPoly)
{
    // Allocate matrices A and B
    float** A = (float**)malloc((degree + 1) * sizeof(float*));
    for (int32_t i = 0; i <= degree; ++i) {
        A[i] = (float*)malloc((degree + 1) * sizeof(float));
    }
    float* B = (float*)malloc((degree + 1) * sizeof(float));

    // Initialise matrices A and B
    for (int32_t i = 0; i <= degree; ++i) {
        B[i] = 0.0;
        for (int32_t j = 0; j <= degree; ++j) {
            A[i][j] = 0.0;
            for (int32_t k = 0; k < numPoints; ++k) {
                A[i][j] += pow(x[k], i + j);
            }
        }
        for (int32_t k = 0; k < numPoints; ++k) {
            B[i] += y[k] * pow(x[k], i);
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
float evaluatePolynomial(const Polynomial* poly, float x)
{
    float result = 0.0;
    for (int32_t i = 0; i <= poly->degree; ++i) {
        result += poly->coefficients[i] * pow(x, i);
    }
    /* Threshold to 0, avoid negative values */
    if (result <= 1.0f) {
        result = 0.0f;
    }
    return (result);
}
