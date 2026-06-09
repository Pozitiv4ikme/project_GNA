#ifndef SINGLEFILEMODE_HPP
#define SINGLEFILEMODE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "Parameters.h"
#include "ParamtersUtils.hpp"
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"
#include "data/GraphParser.hpp"
#include "AlgorithmDispatcher.hpp"

// universal function to execute the whole graph processing pipeline for a given representation type
template <typename GraphRepresentation>
void execute_graph_pipeline(const std::string& repsentationName) {
    GraphRepresentation graph;

    // MST uses undirected graphs, SP and MF use directed graphs (Point 12-d)
    bool isDirected = (Parameters::problem != Parameters::Problems::mst);

    std::cout << "\n----------------------------------------\n";
    std::cout << " Processing Representation: " << repsentationName << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "[INFO] Loading graph from file: " << Parameters::inputFile << "\n";

    // attempt to load the file into the current representation template
    if (GraphParser::loadFromFile(Parameters::inputFile, graph, isDirected)) {
        
        // 1. Output everything to CONSOLE first
        std::cout << "[INFO] Graph successfully loaded. Current representation state:";
        graph.print(std::cout); // Passing std::cout explicitly

        std::cout << "\n[INFO] Running task: " << problem_to_string(Parameters::problem) << "\n";
        std::cout << "[INFO] Selected execution target: " << algorithm_to_string(Parameters::algorithm) << "\n";
        
        // run selected algorithm(s) and print solution to console
        bool success = run_selected_graph_algorithm<GraphRepresentation>(graph, std::cout);
        
        // 2. Output exactly the same structure and solution to FILE (Instructor's Requirement)
        if (success && !Parameters::outputFile.empty()) {
            std::ofstream outFile(Parameters::outputFile, std::ios::app); // Append to file each time in single mode
            
            if (outFile.is_open()) {
                outFile << "========================================\n";
                outFile << "        SINGLE FILE MODE REPORT        \n";
                outFile << "========================================\n";
                outFile << "Problem Task: " << problem_to_string(Parameters::problem) << "\n";
                outFile << "Selected Algorithm: " << algorithm_to_string(Parameters::algorithm) << "\n";
                outFile << "Input Source File: " << Parameters::inputFile << "\n";
                
                // Write graph topology structure to file
                graph.print(outFile);
                
                // Write computed algorithm solution details to file
                run_selected_graph_algorithm<GraphRepresentation>(graph, outFile);
                
                outFile.close();
                std::cout << "[SUCCESS] Results successfully written to file: " << Parameters::outputFile << "\n";
            } else {
                std::cerr << "[ERROR] Could not open or create output file: " << Parameters::outputFile << "\n";
            }
        }

        if (!success) {
            std::cout << "[ERROR] Execution failed for representation: " << repsentationName << ".\n";
        }
    } else {
        std::cerr << "[ERROR] Failed to load or initialize graph for: " << repsentationName << ".\n";
    }
}

// main entry point dispatcher for Single File Mode
void run_single_file_mode() {
    std::cout << "\n========================================\n";
    std::cout << "        SINGLE FILE MODE ACTIVATED      \n";
    std::cout << "========================================\n";
    
    if (Parameters::problem == Parameters::Problems::undefined) {
        std::cerr << "[ERROR] No problem type specified. Aborting single file mode.\n";
        return;
    }

    // сlear the output file ONCE at the very beginning of the application run
    if (!Parameters::outputFile.empty()) {
        std::ofstream clearFile(Parameters::outputFile, std::ios::out);
        // щpening and immediately closing with std::ios::out ensures a fresh start
        clearFile.close(); 
    }

    if (Parameters::structure == Parameters::Structures::incidenceMatrix) {
        execute_graph_pipeline<IncidenceMatrix>("Incidence Matrix");
    } 
    else if (Parameters::structure == Parameters::Structures::adjacencyList) {
        execute_graph_pipeline<SuccessorList>("Adjacency List");
    } 
    else if (Parameters::structure == Parameters::Structures::allStructures) {
        std::cout << "[INFO] Option 'All Structures' selected. Running both representations sequentially.\n";
        execute_graph_pipeline<IncidenceMatrix>("Incidence Matrix");
        execute_graph_pipeline<SuccessorList>("Adjacency List");
    } 
    else {
        std::cout << "[WARNING] Selected graph structure is undefined or unsupported!\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "       FINISHED SINGLE FILE MODE        \n";
    std::cout << "========================================\n";
}

#endif