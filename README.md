# PolyFit

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Top Language](https://img.shields.io/github/languages/top/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit)
[![Issues](https://img.shields.io/github/issues/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/issues)
[![Pull Requests](https://img.shields.io/github/issues-pr/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/pulls)

A lightweight C library for fitting polynomials to data—fast, simple, and no external dependencies.

## What is it?

**PolyFit** is a small, embeddable C library for polynomial regression. 
It helps you fit a curve through a set of data points so you can interpolate or extrapolate with minimal effort. 

Use PolyFit for signal smoothing, approximating system behavior, or forming simple transfer-function polynomials. Include the two source files and call the API; no extra setup required.

---

## Where It’s Useful

Here's a few real-world uses:

### Signal Processing
Useful in cleaning up noisy data e.g., smoothing biomedical signals like EEGs.

### Robotics & Navigation
Model sensor noise, trajectory paths, or estimate system motion with smooth, continuous polynomials.

### Control Systems
Originally built to model dynamic systems using polynomial-based transfer functions. Great for:
- Root locus
- Frequency response
- Pole-zero plots

---

## Features

- Polynomial curve fitting with configurable order
- Supports both interpolation and extrapolation
- Single-file: just drop in `polyfit.c` and `polyfit.h`
- No dependencies beyond the standard library

---

## Quickstart

### Clone the Repo

```bash
git clone https://github.com/FinOrr/polyfit.git
````

Then just copy `polyfit.c` and `polyfit.h` into your project.

### Include It

```c
#include "polyfit.h"
```

### Basic Usage

**Example 1: Simple linear fit (recommended)**

```c
#include "polyfit.h"
#include <stdio.h>

int main(void) {
    // Your data points
    float x[] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float y[] = {2.1, 3.9, 6.1, 8.0, 9.9};

    // Fit a linear polynomial (degree 1)
    Polynomial *poly = polyfit(x, y, 5, 1, NULL);
    if (poly) {
        float result;
        polyfit_evaluate(poly, 6.0f, &result);
        printf("f(6.0) = %f\n", result);  // Predict at x=6
        polyfit_free(poly);
    }

    return 0;
}
```

**Example 2: One-shot evaluation (no memory management)**

```c
// Fit and evaluate in one call - perfect for quick predictions
float x[] = {0.0, 1.0, 2.0, 3.0};
float y[] = {1.0, 2.0, 5.0, 10.0};
float result;

if (polyfit_eval_at(x, y, 4, 2, 4.0f, &result) == POLYFIT_SUCCESS) {
    printf("f(4.0) = %f\n", result);
}
```

**Example 3: Extract coefficients**

```c
Polynomial *poly = polyfit(x, y, 5, 2, NULL);
if (poly) {
    float coeffs[3];  // degree + 1
    if (polyfit_get_coefficients(poly, coeffs, 3) == POLYFIT_SUCCESS) {
        printf("y = %.2f + %.2f*x + %.2f*x^2\n",
               coeffs[0], coeffs[1], coeffs[2]);
    }
    polyfit_free(poly);
}
```

**Example 4: Error handling**

```c
polyfit_error_t err;
Polynomial *poly = polyfit(x, y, num_points, degree, &err);
if (!poly) {
    fprintf(stderr, "Error: %s\n", polyfit_error_string(err));
    return -1;
}
// Use polynomial...
polyfit_free(poly);
```

### Integration

Just copy these two files into your project:
- `polyfit.c`
- `polyfit.h`

Then compile with your source code:

```bash
gcc -o myapp main.c polyfit.c -lm
```

---

## Documentation

More detailed info and examples are available on the [Wiki](https://github.com/FinOrr/polyfit/wiki).

---

## Contributing

Found a bug? Want to improve performance or add features? PRs welcome, just read the [contributing guide](CONTRIBUTING.md) first.

---

## License

MIT. See [LICENSE](LICENSE) for full details.

---

## Questions?

Open an [issue](https://github.com/FinOrr/polyfit/issues) if you get stuck or want to ask anything.
