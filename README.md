# polyfit

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Top Language](https://img.shields.io/github/languages/top/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit)
[![Issues](https://img.shields.io/github/issues/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/issues)
[![Pull Requests](https://img.shields.io/github/issues-pr/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/pulls)

A lightweight C library for fitting polynomials to data: fast, simple, and no external dependencies.

## API

```c
// Fit and return a polynomial (caller must free)
Polynomial *polyfit(const float *x, const float *y, int32_t num_points,
                    int32_t degree, polyfit_error_t *error);

// Evaluate a fitted polynomial at x
polyfit_error_t polyfit_evaluate(const Polynomial *poly, float x, float *result);

// Fit and evaluate at a single point (no memory management required)
polyfit_error_t polyfit_eval_at(const float *x, const float *y,
                                int32_t num_points, int32_t degree,
                                float eval_x, float *result);

// Copy coefficients out (ascending order: coeffs[0] + coeffs[1]*x + ...)
polyfit_error_t polyfit_get_coefficients(const Polynomial *poly,
                                         float *coeffs, int32_t size);

void polyfit_free(Polynomial *poly);
const char *polyfit_error_string(polyfit_error_t error);
```

Supported degrees: 0 - 10. Requires more data points than the polynomial degree.

## Usage

```c
#include "polyfit.h"
#include <stdio.h>

int main(void) {
    float x[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    float y[] = {2.1f, 3.9f, 6.1f, 8.0f, 9.9f};

    Polynomial *poly = polyfit(x, y, 5, 1, NULL);
    if (poly) {
        float result;
        polyfit_evaluate(poly, 6.0f, &result);
        printf("f(6.0) = %f\n", result);
        polyfit_free(poly);
    }

    // Or fit and evaluate in one call
    float result;
    polyfit_eval_at(x, y, 5, 1, 6.0f, &result);

    return 0;
}
```

```bash
gcc -o myapp main.c polyfit.c -lm
```

## Contributing

PRs welcome -- see [CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT. See [LICENSE](LICENSE).
