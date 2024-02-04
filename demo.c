#include <stdio.h>
#include "polyFit.h"

int main(void) {
  ////////////////////////////////////////////////////
  // Demo: Modelling an x^2 parabola
  // There is typically 4 steps to using the library.
  ////////////////////////////////////////////////////

  // Step 1: Initialise the polynomial
  // You'll need to define the degree of the polynomial. This will be optimised
  // in the future. For now, we need to manually assign the degree ('order') of
  // the polynomial In this example we want to fit an x^3 curve, so we use
  // degree=2.
  int32_t degree = 3;
  Polynomial *poly = initPolynomial(degree);

  // Step 2: Fit the polynomial coefficients to your data.
  // You will need an input array of samples (x and y components), these are
  // used used to fit the polynomial. In this demo, we're modelling an x^3
  // parabola, so let's record the first 4 components of this curve.
  float xData[] = {0, 2, 4, 5};
  float yData[] = {0, 8, 64, 125};

  // Basic validation, confirm each x sample has a corresponding y sample.
  int32_t numXSamples = (int32_t)(sizeof(xData) / sizeof(xData[0]));
  int32_t numYSamples = (int32_t)(sizeof(yData) / sizeof(yData[0]));
  if (numXSamples == numYSamples) {
    // Now we fit the polynomial to the dataset with the following function call
    leastSquaresPolynomialRegression(xData, yData, numXSamples, degree, poly);
  } else {
    // Not the most gracious of error handling, but you get the idea.
    exit(EXIT_FAILURE);
  }

  // Step 3: Now you can interpolate or extrapolate any point on the curve.
  // Demo output text showing extrapolation / interpolation of the x^3 curve.
  printf("Testing polynomial for various x values:\n");
  printf("| %-10s | %-10s |\n", "x", "y (y=x^2)");
  printf("|------------|------------|\n");

  // To find a data point on the curve, pass the polynomial and the desired x-
  // component to the evaluatePolynomial() function. Let's test 20 locations:
  for (float x = -10; x <= 10; x += 1) {
    float y = evaluatePolynomial(poly, x);
    printf("| %-10.2f | %-10.2f |\n", x, y);
  }

  // Step 4: Free the memory when we're done to prevent memory leaks.
  freePolynomial(poly);

  return (EXIT_SUCCESS);
}
