# PolyFit

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![GitHub top language](https://img.shields.io/github/languages/top/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit)
[![GitHub issues](https://img.shields.io/github/issues/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/FinOrr/polyfit.svg)](https://github.com/FinOrr/polyfit/pulls)


## Overview

**PolyFit** is a C library designed to facilitate accurate polynomial fitting for curve approximation, allowing for interpolation and extrapolation of data points. With just a few input datapoints, you can model your system quickly and efficiently.

## Use Case Scenarios

Sound a bit asbtract? No worries, here's some examples of where PolyFit might solve a problem you have.

**Curve Fitting in Data Analysis** 
- In climate science research, polynomial curve fitting is applied to analyse historical temperature data and predict future climate trends, aiding in understanding global warming patterns.

**Image Processing and Computer Vision** 
- Polynomials are utilised in computer vision tasks, such as facial recognition systems, where they help map facial features accurately for identification purposes.

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

**Function Approximation in Optimisation**
- Civil engineers use polynomial approximations to model stress-strain relationships in structural materials for bridge design. By approximating complex material behavior with polynomials, engineers can optimise structural designs for safety and durability under various loading conditions.

**Transform Functions and Control Systems**
- Polynomial models are vital in representing system transfer functions, which describe the relationship between inputs and outputs of a dynamic system. By mapping polynomials to transfer functions, engineers can model the behavior of diverse systems across various domains, including aerospace, automotive, and industrial control.
- For instance, in control systems engineering, polynomials are transformed into transfer functions to characterise the dynamics of physical systems, such as aircraft, vehicles, and manufacturing processes! 
- These transfer functions capture the system's response to inputs and enable engineers to design control algorithms to achieve desired performance objectives, such as stability, responsiveness, and robustness.
- With transfer functions derived from polynomials, engineers can perform advanced control systems analysis techniques, such as Root-Locus, frequency response analysis, and pole-zero analysis.

**Control System Applications**
- In industrial automation, polynomial controllers regulate motors by translating input commands into precise motor outputs. 
- By applying specific inputs and measuring corresponding outputs, such as speed or torque, engineers optimise manufacturing processes. 
- For example, in controlling fluid pump outputs, polynomial controllers enable adjustments to achieve desired pressure levels or flow rates, enhancing efficiency and product quality.

## Features

- Polynomial fitting for curve approximation.
- Interpolation and extrapolation functionality for predicting additional data points.
- Lightweight and easy-to-integrate into existing C projects.

## Usage

### Installation

Clone the repository:

```bash
git clone https://github.com/FinOrr/polyfit.git
```

Copy the polyFit.c and polyFit.h files into your project directory.

Include the polyFit.h header file in your source files where you want to use the polynomial regression functionality.

```c
#include "polyFit.h"
```

### Example

See the demo.c program for example usage.
To build the demo, you'll need CMake installed.

Open a terminal, navigate to the directory containing your source files and the CMakeLists.txt, and run the following commands:

```bash
mkdir build
cd build
cmake ..
```

This will generate the build files. Once the files are generated, you can build your project by running:
```bash
make
```

This will compile your source files and create the executable. If you want to clean the generated files, you can run:
```bash
make clean
```

The resulting executable will be in the build directory.

## Documentation

Detailed documentation can be found in the [Wiki](https://github.com/FinOrr/polyfit/wiki).

## Contributing

Contributions are welcome! Please check the [Contribution Guidelines](CONTRIBUTING.md) before making a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
  
## Contact

For any inquiries or support, please use the [Issues](https://github.com/FinOrr/polyfit/issues) page.
 
