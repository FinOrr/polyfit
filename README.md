# PolyFit

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![GitHub top language](https://img.shields.io/github/languages/top/FinOrr/c-polyfit.svg)](https://github.com/FinOrr/c-polyfit)
[![GitHub issues](https://img.shields.io/github/issues/FinOrr/c-polyfit.svg)](https://github.com/FinOrr/c-polyfit/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/FinOrr/c-polyfit.svg)](https://github.com/FinOrr/c-polyfit/pulls)


## Overview

**PolyFit** is a C library designed to facilitate accurate polynomial fitting for curve approximation, allowing for interpolation and extrapolation of data points. Enhance your data analysis capabilities with this efficient and versatile tool!

## Features

- Polynomial fitting for curve approximation.
- Interpolation and extrapolation functionality for predicting additional data points.
- Lightweight and easy-to-integrate into existing C projects.

## Usage

### Installation

Clone the repository:

```bash
git clone https://github.com/your-username/PolyFit.git
```

Build the library:

```bash
cd c-polyfit
make
```

### Usage

The Makefile creates a shared library (libpolyfit.so) from your source file (polyFit.c).

To use the library in another project, you'll need to compile with the -L flag to specify the library path and the -l flag to specify the library name (without the "lib" prefix and ".so" extension). For example:

```bash

CC = gcc
CFLAGS = -Wall -Wextra -g
LIB_PATH = /path/to/library
LIB_NAME = polyfit
SRC = your_source_file.c
EXECUTABLE = your_executable

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $< -L$(LIB_PATH) -l$(LIB_NAME)
```

To get started in using the library in your own projects, simply:
- Replace /path/to/library with the actual path to your library;
- Ensure to link polyfit with the actual library name (without the "lib" prefix and ".so" extension).
Please note that his assumes that the library (libpolyfit.so) is located in /path/to/library.

### Example

```c
Examples coming soon...
```

## Documentation

Detailed documentation can be found in the [Wiki](https://github.com/FinOrr/c-polyfit/wiki).

## Contributing

Contributions are welcome! Please check the [Contribution Guidelines](CONTRIBUTING.md) before making a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
  
## Contact

For any inquiries or support, please use the [Issues](https://github.com/FinOrr/c-polyfit/issues) page.
 
