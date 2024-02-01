#include <stdio.h>

#include "polyFit.h"

int main(void) {
  ////////////////////////////////////////////////////
  // Demo: Modelling an x^2 parabola
  // There is typically 4 steps to using the library.
  ////////////////////////////////////////////////////

  // Step 1: Initialise the polynomial
  // You'll need to define the order of the polynomial; this could be optimised
  // in the future. For now we need to manually assign the degree ('order') of
  // the polynomial In this example we want to fit an x^2 curve, so we use
  // order=2.
  int32_t degree = 3;
  Polynomial *poly = initPolynomial(degree);

  // Step 2: Fit the polynomial coefficients to your data.
  // You will need an input array of samples (x and y components), these are
  // used used to fit the polynomial. In this demo, we're modelling an x^2
  // parabola, so let's record the first 4 components
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

  // Step 3: Now you can interpolate or extrapolate any point on the fitted
  // curve! Let's imagine we're at point x=20, we'd expect the y co-ordinate to
  // be 400. float x = 20; float y = evaluatePolynomial(poly, x);
  // printf("Extrapolated with degree [%d].\nWhen x=20, we expect y=[%.2f].",
  // degree, y);

  printf("Testing polynomial for various x values:\n");
  printf("| %-10s | %-10s |\n", "x", "y (y=x^2)");
  printf("|------------|------------|\n");

  for (float x = -10; x <= 10; x += 1) {
    float y = evaluatePolynomial(poly, x);
    printf("| %-10.2f | %-10.2f |\n", x, y);
  }

  // Extrapolation example
  float extrapolation_x = 20;
  float extrapolation_y = evaluatePolynomial(poly, extrapolation_x);
  printf("\nExtrapolation Result:\n");
  printf("  Polynomial Degree: %d\n", degree);
  printf("  Extrapolation Point: x=%.2f\n", extrapolation_x);
  printf("  Expected y Value: %.2f\n", extrapolation_y);

  // Step 4: Free the memory when we're done to prevent memory leaks.
  freePolynomial(poly);

  return (EXIT_SUCCESS);
}
