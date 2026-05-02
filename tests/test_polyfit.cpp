extern "C" {
#include "polyfit.h"
}

#include <gtest/gtest.h>
#include <cmath>

/*============================================================================*/
/* SHARED TEST DATA                                                           */
/*============================================================================*/

// y = 2x + 1 over 5 points
static const float kLinX[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
static const float kLinY[] = {1.0f, 3.0f, 5.0f, 7.0f, 9.0f};
static const int   kLinN   = 5;

// y = x^2 over 9 points
static const float kQuadX[] = {-3.0f, -2.0f, -1.0f, 0.0f, 1.0f,
                                 2.0f,  3.0f,  4.0f, 5.0f};
static const float kQuadY[] = { 9.0f,  4.0f,  1.0f, 0.0f, 1.0f,
                                 4.0f,  9.0f, 16.0f, 25.0f};
static const int   kQuadN   = 9;

/*============================================================================*/
/* INIT / FREE                                                                */
/*============================================================================*/

TEST(PolyfitInit, ValidDegrees) {
    Polynomial *p = polyfit_init(0);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->degree, 0);
    EXPECT_TRUE(p->is_valid);
    polyfit_free(p);

    p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->degree, 1);
    polyfit_free(p);

    p = polyfit_init(POLYFIT_MAX_DEGREE);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->degree, POLYFIT_MAX_DEGREE);
    polyfit_free(p);
}

TEST(PolyfitInit, NegativeDegreeReturnsNull) {
    EXPECT_EQ(polyfit_init(-1), nullptr);
}

TEST(PolyfitInit, ExceedsMaxDegreeReturnsNull) {
    EXPECT_EQ(polyfit_init(POLYFIT_MAX_DEGREE + 1), nullptr);
}

TEST(PolyfitFree, NullIsSafe) {
    EXPECT_NO_THROW(polyfit_free(nullptr));
}

/*============================================================================*/
/* IS VALID                                                                   */
/*============================================================================*/

TEST(PolyfitIsValid, Null) {
    EXPECT_FALSE(polyfit_is_valid(nullptr));
}

TEST(PolyfitIsValid, ValidPolyAfterInit) {
    Polynomial *p = polyfit_init(2);
    ASSERT_NE(p, nullptr);
    EXPECT_TRUE(polyfit_is_valid(p));
    polyfit_free(p);
}

TEST(PolyfitIsValid, NullCoefficientsIsInvalid) {
    Polynomial p;
    p.coefficients = nullptr;
    p.degree = 1;
    p.is_valid = true;
    EXPECT_FALSE(polyfit_is_valid(&p));
}

/*============================================================================*/
/* ERROR STRINGS                                                              */
/*============================================================================*/

TEST(PolyfitErrorString, AllCodesReturnNonNull) {
    EXPECT_STREQ(polyfit_error_string(POLYFIT_SUCCESS), "Success");
    EXPECT_NE(polyfit_error_string(POLYFIT_ERROR_NULL_POINTER), nullptr);
    EXPECT_NE(polyfit_error_string(POLYFIT_ERROR_INVALID_DEGREE), nullptr);
    EXPECT_NE(polyfit_error_string(POLYFIT_ERROR_MEMORY_ALLOC), nullptr);
    EXPECT_NE(polyfit_error_string(POLYFIT_ERROR_SINGULAR_MATRIX), nullptr);
    EXPECT_NE(polyfit_error_string(POLYFIT_ERROR_INSUFFICIENT_POINTS), nullptr);
    EXPECT_NE(polyfit_error_string(POLYFIT_ERROR_INVALID_INPUT), nullptr);
}

TEST(PolyfitErrorString, UnknownCodeReturnsNonNull) {
    EXPECT_NE(polyfit_error_string((polyfit_error_t)999), nullptr);
}

/*============================================================================*/
/* LEAST SQUARES CORE                                                         */
/*============================================================================*/

TEST(PolyfitLeastSquares, LinearFit) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(kLinX, kLinY, kLinN, 1, p), POLYFIT_SUCCESS);
    // y = 1 + 2x → coeffs[0]=1, coeffs[1]=2
    EXPECT_NEAR(p->coefficients[0], 1.0f, 1e-4f);
    EXPECT_NEAR(p->coefficients[1], 2.0f, 1e-4f);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, QuadraticFit) {
    Polynomial *p = polyfit_init(2);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(kQuadX, kQuadY, kQuadN, 2, p), POLYFIT_SUCCESS);
    // y = x^2 → coeffs[0]=0, coeffs[1]=0, coeffs[2]=1
    EXPECT_NEAR(p->coefficients[0], 0.0f, 1e-4f);
    EXPECT_NEAR(p->coefficients[1], 0.0f, 1e-4f);
    EXPECT_NEAR(p->coefficients[2], 1.0f, 1e-4f);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, NullXReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(nullptr, kLinY, kLinN, 1, p),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, NullYReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(kLinX, nullptr, kLinN, 1, p),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, NullResultReturnsError) {
    EXPECT_EQ(polyfit_least_squares(kLinX, kLinY, kLinN, 1, nullptr),
              POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitLeastSquares, InsufficientPoints) {
    Polynomial *p = polyfit_init(2);
    ASSERT_NE(p, nullptr);
    // num_points must be > degree; 2 points for degree 2 is not enough
    EXPECT_EQ(polyfit_least_squares(kLinX, kLinY, 2, 2, p),
              POLYFIT_ERROR_INSUFFICIENT_POINTS);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, NegativeDegreeReturnsError) {
    Polynomial *p = polyfit_init(0);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(kLinX, kLinY, kLinN, -1, p),
              POLYFIT_ERROR_INVALID_DEGREE);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, ExceedsMaxDegreeReturnsError) {
    Polynomial *p = polyfit_init(0);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(kLinX, kLinY, kLinN, POLYFIT_MAX_DEGREE + 1, p),
              POLYFIT_ERROR_INVALID_DEGREE);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, NaNInputRejected) {
    float xi[] = {1.0f, 2.0f, 3.0f};
    float yi[] = {0.0f / 0.0f, 2.0f, 3.0f};  // NaN
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(xi, yi, 3, 1, p), POLYFIT_ERROR_INVALID_INPUT);
    polyfit_free(p);
}

TEST(PolyfitLeastSquares, InfInputRejected) {
    float xi[] = {1.0f, 2.0f, 3.0f};
    float yi[] = {1.0f / 0.0f, 2.0f, 3.0f};  // +Inf
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_least_squares(xi, yi, 3, 1, p), POLYFIT_ERROR_INVALID_INPUT);
    polyfit_free(p);
}

/*============================================================================*/
/* EVALUATE                                                                   */
/*============================================================================*/

TEST(PolyfitEvaluate, LinearAtKnownPoints) {
    Polynomial *p = polyfit(kLinX, kLinY, kLinN, 1, nullptr);
    ASSERT_NE(p, nullptr);

    float result;
    EXPECT_EQ(polyfit_evaluate(p, 0.0f, &result), POLYFIT_SUCCESS);
    EXPECT_NEAR(result, 1.0f, 1e-4f);
    EXPECT_EQ(polyfit_evaluate(p, 4.0f, &result), POLYFIT_SUCCESS);
    EXPECT_NEAR(result, 9.0f, 1e-4f);

    polyfit_free(p);
}

TEST(PolyfitEvaluate, NullPolyReturnsError) {
    float result;
    EXPECT_EQ(polyfit_evaluate(nullptr, 1.0f, &result), POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitEvaluate, NullResultReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_evaluate(p, 1.0f, nullptr), POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitEvaluate, SmallLegitimateValueNotSnappedToZero) {
    // y = 5e-7 * x — values well below the old 1e-6 absolute snap threshold
    float xs[] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    float ys[] = {0.0f, 5e-7f, 10e-7f, 15e-7f, 20e-7f};
    Polynomial *p = polyfit(xs, ys, 5, 1, nullptr);
    ASSERT_NE(p, nullptr);

    float result;
    EXPECT_EQ(polyfit_evaluate(p, 1.0f, &result), POLYFIT_SUCCESS);
    EXPECT_NEAR(result, 5e-7f, 1e-8f);  // must NOT be zero

    polyfit_free(p);
}

/*============================================================================*/
/* CONVENIENCE FUNCTIONS                                                      */
/*============================================================================*/

TEST(PolyfitConvenience, PolyfitLinearFit) {
    polyfit_error_t err;
    Polynomial *p = polyfit(kLinX, kLinY, kLinN, 1, &err);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(err, POLYFIT_SUCCESS);
    EXPECT_NEAR(p->coefficients[0], 1.0f, 1e-4f);
    EXPECT_NEAR(p->coefficients[1], 2.0f, 1e-4f);
    polyfit_free(p);
}

TEST(PolyfitConvenience, PolyfitReturnsNullOnNullInput) {
    polyfit_error_t err;
    Polynomial *p = polyfit(nullptr, kLinY, kLinN, 1, &err);
    EXPECT_EQ(p, nullptr);
    EXPECT_EQ(err, POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitConvenience, EvalAtQuadratic) {
    float result;
    EXPECT_EQ(polyfit_eval_at(kQuadX, kQuadY, kQuadN, 2, 4.0f, &result),
              POLYFIT_SUCCESS);
    EXPECT_NEAR(result, 16.0f, 1e-3f);
}

TEST(PolyfitConvenience, EvalAtNullResultReturnsError) {
    EXPECT_EQ(polyfit_eval_at(kQuadX, kQuadY, kQuadN, 2, 4.0f, nullptr),
              POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitConvenience, GetCoefficients) {
    Polynomial *p = polyfit(kLinX, kLinY, kLinN, 1, nullptr);
    ASSERT_NE(p, nullptr);

    float coeffs[2];
    EXPECT_EQ(polyfit_get_coefficients(p, coeffs, 2), POLYFIT_SUCCESS);
    EXPECT_NEAR(coeffs[0], 1.0f, 1e-4f);
    EXPECT_NEAR(coeffs[1], 2.0f, 1e-4f);

    polyfit_free(p);
}

TEST(PolyfitConvenience, GetCoefficientsBufferTooSmall) {
    Polynomial *p = polyfit(kLinX, kLinY, kLinN, 1, nullptr);
    ASSERT_NE(p, nullptr);

    float coeffs[1];  // degree=1 needs 2 elements
    EXPECT_EQ(polyfit_get_coefficients(p, coeffs, 1), POLYFIT_ERROR_INVALID_INPUT);

    polyfit_free(p);
}

/*============================================================================*/
/* RESIDUALS                                                                  */
/*============================================================================*/

TEST(PolyfitResiduals, PerfectFitNearZero) {
    Polynomial *p = polyfit(kQuadX, kQuadY, kQuadN, 2, nullptr);
    ASSERT_NE(p, nullptr);

    float residuals[9];
    EXPECT_EQ(polyfit_compute_residuals(p, kQuadX, kQuadY, kQuadN, residuals),
              POLYFIT_SUCCESS);
    for (int i = 0; i < kQuadN; i++) {
        EXPECT_NEAR(residuals[i], 0.0f, 1e-3f) << "residuals[" << i << "]";
    }

    polyfit_free(p);
}

TEST(PolyfitResiduals, NullPolyReturnsError) {
    float residuals[5];
    EXPECT_EQ(polyfit_compute_residuals(nullptr, kLinX, kLinY, kLinN, residuals),
              POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitResiduals, NullXReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    float residuals[5];
    EXPECT_EQ(polyfit_compute_residuals(p, nullptr, kLinY, kLinN, residuals),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitResiduals, NullYReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    float residuals[5];
    EXPECT_EQ(polyfit_compute_residuals(p, kLinX, nullptr, kLinN, residuals),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitResiduals, NullResidualsOutputReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_compute_residuals(p, kLinX, kLinY, kLinN, nullptr),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

/*============================================================================*/
/* R-SQUARED                                                                  */
/*============================================================================*/

TEST(PolyfitRSquared, PerfectFitIsOne) {
    Polynomial *p = polyfit(kQuadX, kQuadY, kQuadN, 2, nullptr);
    ASSERT_NE(p, nullptr);

    float r2;
    EXPECT_EQ(polyfit_r_squared(p, kQuadX, kQuadY, kQuadN, &r2), POLYFIT_SUCCESS);
    EXPECT_NEAR(r2, 1.0f, 1e-5f);

    polyfit_free(p);
}

TEST(PolyfitRSquared, WrongDegreeGivesLowerScore) {
    // Fit degree-1 to quadratic data — R² should be well below 1
    Polynomial *p = polyfit(kQuadX, kQuadY, kQuadN, 1, nullptr);
    ASSERT_NE(p, nullptr);

    float r2;
    EXPECT_EQ(polyfit_r_squared(p, kQuadX, kQuadY, kQuadN, &r2), POLYFIT_SUCCESS);
    EXPECT_LT(r2, 0.99f);

    polyfit_free(p);
}

TEST(PolyfitRSquared, NullPolyReturnsError) {
    float r2;
    EXPECT_EQ(polyfit_r_squared(nullptr, kLinX, kLinY, kLinN, &r2),
              POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitRSquared, NullXReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    float r2;
    EXPECT_EQ(polyfit_r_squared(p, nullptr, kLinY, kLinN, &r2),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitRSquared, NullYReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    float r2;
    EXPECT_EQ(polyfit_r_squared(p, kLinX, nullptr, kLinN, &r2),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

TEST(PolyfitRSquared, NullOutputReturnsError) {
    Polynomial *p = polyfit_init(1);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(polyfit_r_squared(p, kLinX, kLinY, kLinN, nullptr),
              POLYFIT_ERROR_NULL_POINTER);
    polyfit_free(p);
}

/*============================================================================*/
/* BEST DEGREE AUTO-SELECTION                                                 */
/*============================================================================*/

TEST(PolyfitBestDegree, SelectsLinearForLinearData) {
    int32_t deg;
    Polynomial *p = polyfit_best_degree(kLinX, kLinY, kLinN, 3, &deg, nullptr);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(deg, 1);
    polyfit_free(p);
}

TEST(PolyfitBestDegree, SelectsQuadraticForQuadraticData) {
    int32_t deg;
    Polynomial *p = polyfit_best_degree(kQuadX, kQuadY, kQuadN, 5, &deg, nullptr);
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(deg, 2);
    polyfit_free(p);
}

TEST(PolyfitBestDegree, ReturnedPolyEvaluatesCorrectly) {
    int32_t deg;
    Polynomial *p = polyfit_best_degree(kQuadX, kQuadY, kQuadN, 4, &deg, nullptr);
    ASSERT_NE(p, nullptr);

    float result;
    EXPECT_EQ(polyfit_evaluate(p, 4.0f, &result), POLYFIT_SUCCESS);
    EXPECT_NEAR(result, 16.0f, 1e-2f);

    polyfit_free(p);
}

TEST(PolyfitBestDegree, NullXReturnsError) {
    int32_t deg;
    polyfit_error_t err;
    EXPECT_EQ(polyfit_best_degree(nullptr, kQuadY, kQuadN, 4, &deg, &err), nullptr);
    EXPECT_EQ(err, POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitBestDegree, NullYReturnsError) {
    int32_t deg;
    polyfit_error_t err;
    EXPECT_EQ(polyfit_best_degree(kQuadX, nullptr, kQuadN, 4, &deg, &err), nullptr);
    EXPECT_EQ(err, POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitBestDegree, NullBestDegreeOutputReturnsError) {
    polyfit_error_t err;
    EXPECT_EQ(polyfit_best_degree(kQuadX, kQuadY, kQuadN, 4, nullptr, &err), nullptr);
    EXPECT_EQ(err, POLYFIT_ERROR_NULL_POINTER);
}

TEST(PolyfitBestDegree, InsufficientPoints) {
    // 3 points but max_degree=5 requires at least 6 points
    float xi[] = {0.0f, 1.0f, 2.0f};
    float yi[] = {1.0f, 2.0f, 4.0f};
    int32_t deg;
    polyfit_error_t err;
    Polynomial *p = polyfit_best_degree(xi, yi, 3, 5, &deg, &err);
    EXPECT_EQ(p, nullptr);
    EXPECT_EQ(err, POLYFIT_ERROR_INSUFFICIENT_POINTS);
}

/*============================================================================*/
/* UTILITY FUNCTIONS                                                          */
/*============================================================================*/

TEST(PolyfitPow, ZeroExponent) {
    EXPECT_FLOAT_EQ(polyfit_pow(5.0f, 0),  1.0f);
    EXPECT_FLOAT_EQ(polyfit_pow(0.0f, 0),  1.0f);  // 0^0 = 1 by convention
}

TEST(PolyfitPow, ZeroBase) {
    EXPECT_FLOAT_EQ(polyfit_pow(0.0f, 3), 0.0f);
}

TEST(PolyfitPow, OneBase) {
    EXPECT_FLOAT_EQ(polyfit_pow(1.0f, 99), 1.0f);
}

TEST(PolyfitPow, NegativeOneBase) {
    EXPECT_FLOAT_EQ(polyfit_pow(-1.0f, 2), 1.0f);   // even exponent
    EXPECT_FLOAT_EQ(polyfit_pow(-1.0f, 3), -1.0f);  // odd exponent
}

TEST(PolyfitPow, PositiveExponents) {
    EXPECT_NEAR(polyfit_pow(2.0f, 3),  8.0f,  1e-6f);
    EXPECT_NEAR(polyfit_pow(3.0f, 4), 81.0f,  1e-5f);
}

TEST(PolyfitPow, NegativeExponent) {
    EXPECT_NEAR(polyfit_pow(2.0f, -2), 0.25f, 1e-7f);
}

TEST(PolyfitFabs, Positive)  { EXPECT_FLOAT_EQ(polyfit_fabs(3.14f),  3.14f); }
TEST(PolyfitFabs, Negative)  { EXPECT_FLOAT_EQ(polyfit_fabs(-3.14f), 3.14f); }
TEST(PolyfitFabs, Zero)      { EXPECT_FLOAT_EQ(polyfit_fabs(0.0f),   0.0f);  }
TEST(PolyfitFabs, NaN) {
    float nan_val = 0.0f / 0.0f;
    EXPECT_TRUE(std::isnan(polyfit_fabs(nan_val)));
}

TEST(PolyfitIsNearlyZero, BelowThresholdIsTrue) {
    EXPECT_TRUE(polyfit_is_nearly_zero(1e-8f, 1e-6f));
}

TEST(PolyfitIsNearlyZero, AboveThresholdIsFalse) {
    EXPECT_FALSE(polyfit_is_nearly_zero(1e-5f, 1e-6f));
}

TEST(PolyfitIsNearlyZero, ExactlyAtThresholdIsFalse) {
    // strictly less than threshold required
    EXPECT_FALSE(polyfit_is_nearly_zero(1e-6f, 1e-6f));
}
