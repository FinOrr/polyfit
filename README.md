# PolyFit

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![Top Language](https://img.shields.io/github/languages/top/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit)
[![Issues](https://img.shields.io/github/issues/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/issues)
[![Pull Requests](https://img.shields.io/github/issues-pr/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/pulls)

A lightweight C library for fitting polynomials to data—fast, simple, and no external dependencies.

## What is it?

**PolyFit** is a small, embeddable C library for polynomial regression. 
It helps you fit a curve through a set of data points so you can interpolate or extrapolate with minimal effort. 

Whether you're smoothing a signal, modeling system behavior, or generating transfer functions, polyfit is built to drop into your project and get out of your way.

---

## Where It’s Useful

Here's a few real-world uses:

### 📈 Signal Processing
Useful in cleaning up noisy data—e.g., smoothing biomedical signals like EEGs.

### 🤖 Robotics & Navigation
Model sensor noise, trajectory paths, or estimate system motion with smooth, continuous polynomials.

### 🛠 Control Systems
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

### Build the Demo (Optional)

There's a `demo.c` to try things out:

```bash
mkdir build
cd build
cmake ..
make
```

To clean up:

```bash
make clean
```

The compiled binary will land in the `build/` folder.

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
