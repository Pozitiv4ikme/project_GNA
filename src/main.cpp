#include <iostream>
#include "Parameters.h"
#include "SingleFileMode.hpp"

using namespace Parameters;
using namespace std;

int main(int argc, char **argv) {
    int result = -1;
    
    // Checking if we pass any arguments besides the program name
    if (argc > 1) {
        // Cut argv[0] and pass only real parameters to the library as verified in Project 1
        result = readParameters(argc - 1, argv + 1);
    } else {
        cerr << "Please provide arguments. Try running with --help" << endl;
        Parameters::help();
        return 1;
    }

    if (result == 0) {
        if (runMode == RunModes::help) {
            Parameters::help();
            return 0; 
        }

        // single file verification mode 
        if (runMode == RunModes::singleFile) {
            run_single_file_mode();
        } 
        // statistics gathering benchmark mode 
        else if (runMode == RunModes::benchmark) {
            std::cout << "[INFO] Benchmark mode detected. Execution skeleton is ready.\n";
            // run_benchmark_mode(); 
        }
    } else {
        cerr << "[ERROR] Initialization parameters could not be processed successfully.\n";
        return 1;
    }
    
    return 0;
}