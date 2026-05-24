# Graph Algorithms and Complexity Analysis

This project is developed as part of the **Algorithms and Computational Complexity (AiZO)** course. The goal of the project is to implement various graph representations and algorithms from scratch using C++17, analyze their efficiency, and perform comprehensive performance testing.

## Project Constraints & Requirements

As per the project specification:
* **Language Standard:** C++17.
* **No STL Containers:** Standard library containers like `std::vector` or `std::list` are **strictly prohibited**. All data structures must be implemented manually from scratch with explicit dynamic memory management.
* **Templates:** The algorithms and graph representations must use C++ templates to ensure generic support across different structures.
* **Compilation Safety:** The project must compile with strict flags: `-Wall -Wextra -Werror` (zero warnings allowed).
* **Performance Measurement:** Time measurements must be done in microseconds using `std::chrono`. Tests for each configuration must be executed multiple times (e.g., 50 times) to calculate average, minimum, and maximum execution times.

---

## Project Structure

* `src/` - Main application source files (CLI logic, configuration, file processing).
* `include/` - Header files containing custom templated data structures and graph algorithms.
* `lib/` - External libraries provided by the instructor for command-line argument parsing.
* `tests/` - A custom, isolated unit testing framework to validate data structures and algorithms immediately during development.

---

## How to Build and Run

### Prerequisites
* GCC 15.2.0 (or compatible compiler supporting C++17)
* CMake 3.31.6 or higher

### Building the Project
Create a build directory and compile the targets:
```bash
mkdir build
cd build
cmake ..
make