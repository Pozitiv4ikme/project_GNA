#ifndef SINGLEFILEMODE_HPP
#define SINGLEFILEMODE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
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
        
        // 1. Output graph topology to CONSOLE first
        std::cout << "[INFO] Graph successfully loaded. Current representation state:";
        // graph.print(std::cout);

        std::cout << "\n[INFO] Running task: " << problem_to_string(Parameters::problem) << "\n";
        std::cout << "[INFO] Selected execution target: " << algorithm_to_string(Parameters::algorithm) << "\n";
        
        // 2. Output graph topology to FILE once before running algorithms
        if (!Parameters::outputFile.empty()) {
            std::ofstream outFile(Parameters::outputFile, std::ios::app); // append mode
            if (outFile.is_open()) {
                outFile << "========================================\n";
                outFile << "        SINGLE FILE MODE REPORT        \n";
                outFile << "========================================\n";
                outFile << "Graph Representation: " << repsentationName << "\n";
                outFile << "Problem Task:         " << problem_to_string(Parameters::problem) << "\n";
                outFile << "Input Source File:    " << Parameters::inputFile << "\n";
                
                // graph.print(outFile); // write structure topology layout
                outFile.close();
            }
        }

        // helper lambda function to execute a specific algorithm to both console and file
        auto execute_single_algorithm_target = [&](Parameters::Algorithms specificAlgo) {
            // temporarily swap global parameter to match current router loop
            Parameters::Algorithms backupAlgo = Parameters::algorithm;
            Parameters::algorithm = specificAlgo;

            std::cout << "\n[RUN] Executing: " << algorithm_to_string(specificAlgo) << "...\n";
            
            auto startTime = std::chrono::high_resolution_clock::now();
            
            bool success = run_selected_graph_algorithm<GraphRepresentation>(graph, std::cout);
            
            auto endTime = std::chrono::high_resolution_clock::now();
            
            auto elapsedMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

            if (success) {
                std::cout << "[TIME] Pure execution time: " << elapsedMicros << " us\n";
            }
            
            // Append algorithm solution to the report file if execution succeeded
            if (success && !Parameters::outputFile.empty()) {
                std::ofstream outFile(Parameters::outputFile, std::ios::app);
                if (outFile.is_open()) {
                    outFile << "\n--- Solution: " << algorithm_to_string(specificAlgo) << " ---\n";
                    outFile << "Execution Time: " << elapsedMicros << " us\n"; // Пишемо час у звітний файл
                    run_selected_graph_algorithm<GraphRepresentation>(graph, outFile);
                    outFile.close();
                }
            }

            // restore global parameters configuration state
            Parameters::algorithm = backupAlgo;
        };

        // 3. Routing core execution based on algorithm selection 
        if (Parameters::algorithm == Parameters::Algorithms::allAlgorithms) {
            std::cout << "[INFO] 'All Algorithms' option active. Running all matching solvers sequentially.\n";
            
            if (Parameters::problem == Parameters::Problems::mst) {
                execute_single_algorithm_target(Parameters::Algorithms::prim);
                execute_single_algorithm_target(Parameters::Algorithms::kruskal);
            } 
            else if (Parameters::problem == Parameters::Problems::sp) {
                execute_single_algorithm_target(Parameters::Algorithms::dijkstra);
                execute_single_algorithm_target(Parameters::Algorithms::bellmanFord);
            } 
            else if (Parameters::problem == Parameters::Problems::mf) {
                execute_single_algorithm_target(Parameters::Algorithms::fordFulkerson);
            }
        } else {
            // execute only the single explicitly requested algorithm target
            execute_single_algorithm_target(Parameters::algorithm);
        }

        std::cout << "\n[SUCCESS] Finished processing pipeline for " << repsentationName << ".\n";
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

    // сlear the output file ONCE at the very beginning of the application run to ensure a fresh start
    if (!Parameters::outputFile.empty()) {
        std::ofstream clearFile(Parameters::outputFile, std::ios::out);
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