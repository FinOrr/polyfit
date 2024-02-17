#include <stdio.h>
#include <polyfit/polyfit.h>

void basicDemo(void);

int main(void)
{
	basicDemo();
	return (EXIT_SUCCESS);
}

/**
 * @brief Demonstrates modelling an x^2 parabola using the library.
 *
 * This example illustrates the four main steps involved in using the library to fit a polynomial to
 * data and evaluate it.
 */
void basicDemo(void)
{
	/**
	 * @brief Initializes the polynomial with a given degree.
	 *
	 * This function allocates memory and sets up the internal state of the polynomial
	 * object. The degree parameter specifies the highest power of the polynomial.
	 *
	 * @param degree The degree of the polynomial.
	 *
	 * @return A pointer to the newly created polynomial object, or NULL on failure.
	 */
	int32_t degree = 3;
	Polynomial* poly = initPolynomial(degree);

	/**
	 * @brief Fits the polynomial coefficients to a given dataset.
	 *
	 * This function uses the least squares method to fit the polynomial coefficients
	 * to a set of sample points provided in the xData and yData arrays.
	 *
	 * @param xData An array of x-coordinates of the sample points.
	 * @param yData An array of y-coordinates of the sample points.
	 * @param numSamples The number of samples in the dataset.
	 * @param degree The degree of the polynomial.
	 * @param poly The polynomial object to fit.
	 */
	float xData[] = {0, 2, 4, 5};
	float yData[] = {0, 8, 64, 125};

	int32_t numXSamples = (int32_t)(sizeof(xData) / sizeof(xData[0]));
	int32_t numYSamples = (int32_t)(sizeof(yData) / sizeof(yData[0]));
	if(numXSamples == numYSamples)
	{
		leastSquaresPolynomialRegression(xData, yData, numXSamples, degree, poly);
	}
	else
	{
		// Handle error gracefully
		exit(EXIT_FAILURE);
	}

	/**
	 * @brief Evaluates the polynomial at a given x-value.
	 *
	 * This function calculates the corresponding y-value for a given x-coordinate
	 * based on the fitted polynomial coefficients.
	 *
	 * @param poly The fitted polynomial object.
	 * @param x The x-coordinate to evaluate.
	 *
	 * @return The calculated y-value.
	 */
	printf("Testing polynomial for various x values:\n");
	printf("| %-10s | %-10s |\n", "x", "y (y=x^2)");
	printf("|------------|------------|\n");

	for(float x = -10; x <= 10; x += 1)
	{
		float y = evaluatePolynomial(poly, x);
		printf("| %-10.2f | %-10.2f |\n", x, y);
	}

	/**
	 * @brief Frees the memory allocated for the polynomial object.
	 *
	 * This function deallocates the memory used by the polynomial object to
	 * prevent memory leaks.
	 *
	 * @param poly The polynomial object to free.
	 */
	freePolynomial(poly);
}
