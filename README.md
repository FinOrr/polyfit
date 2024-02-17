# PolyFit

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![GitHub top language](https://img.shields.io/github/languages/top/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit)
[![GitHub issues](https://img.shields.io/github/issues/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/pulls)

## Overview

**PolyFit** is a C library designed to facilitate accurate polynomial fitting for curve approximation, allowing for interpolation and extrapolation of data. With just a few input datapoints, you can model your system quickly and efficiently.

## Features

- Polynomial fitting for curve approximation.

- Interpolation and extrapolation functionality for predicting additional data points.

- Lightweight and easy-to-integrate into existing C projects.

## Table of Contents

1. [Use Case Scenarios](#use-case-scenarios)
2. [Project Status](#project-status)
3. [Getting Started](#getting-started)
    1. [Requirements](#requirements)
        1. [git-lfs](#git-lfs)
        1. [CMake Build System](#cmake-build-system)
    2. [Getting the Source](#getting-the-source)
    3. [Building](#building)
    4. [Testing](#testing)
5. [Documentation](#documentation)
6. [Need Help?](#need-help)
7. [Contributing](#contributing)
8. [Further Reading](#further-reading)
9. [Authors](#authors)
10. [License](#license)
11. [Acknowledgments](#acknowledgements)

## Use Case Scenarios

Sound a bit asbtract? No worries, here's some examples of where PolyFit can help:

**Curve Fitting in Data Analysis** 
- In climate science research, polynomial curve fitting is applied to analyse historical temperature data and predict future climate trends, aiding in understanding global warming patterns.

**Signal Processing and Filtering**
- Biomedical engineers use polynomial filters to remove noise from electroencephalogram (EEG) signals within neuroscience research, improving the detection of brainwave patterns and facilitating the diagnosis of neurological disorders.

**Financial Modeling and Time Series Analysis**
- Financial analysts utilise polynomial regression models to analyse market data and predict prices. For example, in investment banking, polynomial models are employed to forecast future stock prices, aiding investors in making informed trading decisions.

**Robotics and Path Planning**
- Autonomous drones use polynomial trajectories for path planning in search and rescue missions. For instance, in disaster response scenarios, drones navigate through complex environments by following smooth polynomial paths, enabling efficient search operations and rescuing survivors. Think:
  - feature trajectory estimation;
  - motion model for localisation;
  - map building and refinement; and
  - sensor calibration.

**Transform Functions and Control Systems**
- Polynomial models are vital in representing system transfer functions, which describe the relationship between inputs and outputs of a dynamic system. By mapping polynomials to transfer functions, engineers can model the behavior of diverse systems across various domains, including aerospace, automotive, and industrial control.

- Engineers can use transfer functions derived from polynomials to perform advanced control systems analysis techniques, such as Root-Locus, freq response analysis, and pole-zero analysis to achieve desired performance objectives, such as stability, responsiveness, and robustness.

**Control System Applications**
- In industrial automation, polynomial controllers regulate motors by translating input commands into precise motor outputs. 
- By applying specific inputs and measuring corresponding outputs, such as speed or torque, engineers optimise manufacturing processes. 
- For example, in controlling fluid pump outputs, polynomial controllers enable adjustments to achieve desired pressure levels or flow rates, enhancing efficiency and product quality.

**[Back to top](#table-of-contents)**

# Project Status

The current system requires the user to specify the desired order of polynomial to be fit to the data.
Future releases will focus on auto-detecting the most suitable, lowest-order polynomial.

**[Back to top](#table-of-contents)**

## Getting Started

### Requirements

At a minimum you will need:

* [`git-lfs`](https://git-lfs.github.com), which is used to store binary files in this repository
* [CMake](#cmake-build-system) is the build system
* Some kind of compiler for your target system.
    - This repository has been tested with:
        - gcc-7, gcc-8, gcc-9
        - arm-none-eabi-gcc
        - Apple clang
        - Mainline clang

#### git-lfs

This project stores some files using [`git-lfs`](https://git-lfs.github.com).

To install `git-lfs` on Linux:

```
sudo apt install git-lfs
```

To install `git-lfs` on OS X:

```
brew install git-lfs
```

Additional installation instructions can be found on the [`git-lfs` website](https://git-lfs.github.com).

#### CMake Build System

The official way to install CMake is to use the pre-compiled binaries and installers on the [CMake download page](https://cmake.org/download/). You can also [compile CMake from source](https://cmake.org/install/). CMake can also be installed through popular package managers, although they may be slightly behind the latest release available on the website.

You can install CMake with `apt` on Linux/WSL:

```
sudo apt-get install cmake
```

> **Note:** Does this not work? You may need to add an [apt repository](https://apt.kitware.com/).

OS X users can install CMake using [Homebrew](homebrew):

```
brew install cmake
```

You can also use Python's `pip` to install CMake:

```
$ pip3 install cmake
```

Make is the default backend for CMake, but our Makefile interface defaults to Ninja. Ninja is similar in purpose to Make, but provides better performance. 

To install Ninja on Linux & WSL:

```
$ sudo apt install ninja-build
```

To install on OSX:

```
$ brew install ninja
```


**[Back to top](#table-of-contents)**

### Getting the Source

This project uses [`git-lfs`](https://git-lfs.github.com), so please install it before cloning. If you cloned prior to installing `git-lfs`, simply run `git lfs pull` after installation.

This project is hosted on GitHub. You can clone the project directly using this command:

```
git clone --recursive git@github.com:finorr/polyfit.git
```

If you don't clone recursively, be sure to run the following command in the repository or your build will fail:

```
git submodule update --init
```

**[Back to top](#table-of-contents)**

### Building

If Make is installed, the library can be built by issuing the following command:

```
make
```

This will build all targets for your current architecture.

You can clean builds using:

```
make clean
```

You can eliminate the generated `buildresults` folder using:

```
make distclean
```

You can also use  `CMake` directly for compiling.

Create a build output folder:

```
cmake -B buildresults
```

And build all targets by running

```
ninja -C buildresults
```

Cross-compilation is handled using CMake toolchain files. Example files are included in the [`cmake/toolchains/cross`](cmake/toolchains/cross/) folder. You can write your own cross files for your specific processor by defining the toolchain, compilation flags, and linker flags. These settings will be used to compile the project.

Cross-compilation must be configured using the CMake command when creating the build output folder. For example:

```
cmake -B buildresults -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/cross/cortex-m3.cmake
```

Following that, you can run `make` (at the project root) or `ninja` to build the project.

Tests will not be cross-compiled. They will only be built for the native platform.

**[Back to top](#table-of-contents)**

### Testing

The tests for this project are written in CMocka and Catch, which are included as external dependencies and does not need to be installed on your system. You can run the tests by issuing the following command:

```
make test
```

By default, test results are generated for use by the CI server and are formatted in JUnit XML. The test results XML files can be found in `buildresults/test/`.

**[Back to top](#table-of-contents)**

## Documentation

Documentation can be built locally by running the following command:

```
make docs
```

Documentation can be found in `buildresults/docs`, and the root page is `index.html`.

**[Back to top](#table-of-contents)**

## Need help?

For any inquiries or support, please use the [Issues](https://github.com/FinOrr/polyfit/issues) page.

If you need further assistance or have any questions, please file a GitHub issue or [send me an email](mailto:3ikxlgcis@mozmail.com).

## Contributing

If you are interested in contributing to this project, please read our [contributing guidelines](docs/CONTRIBUTING.md).

## Authors

* Polyfit source code - **[Fin Orr](https://github.com/finorr)**
* CMake Repo Design -  **[Phillip Johnston](https://github.com/phillipjohnston)**

## License

See the [LICENSE](LICENSE) file for licensing details.

## Acknowledgments

This repository was built using the [CMake repository template](https://github.com/embeddedartistry/cmake-project-skeleton) from the team at Embedded Artistry.

**[Back to top](#table-of-contents)**
