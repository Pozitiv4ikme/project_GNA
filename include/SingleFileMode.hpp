#ifndef SINGLEFILEMODE_HPP
#define SINGLEFILEMODE_HPP

#include <iostream>
#include <string>
#include "Parameters.h"
#include "ParamtersUtils.hpp"
#include "IncidenceMatrix.hpp"
#include "SuccessorList.hpp"
#include "GraphParser.hpp"

// forward declaration for the algorithm executor which will be implemented in future 
// for now using maybe_unused attribute to avoid warnings
template <typename GraphRepresentation>
bool run_selected_graph_algorithm([[maybe_unused]] GraphRepresentation& graph) {
    // temporary stub
    std::cout << "[Dispatcher] Algorithm stub executed successfully.\n";
    return true; 
}

// universal function to execute the whole graph processing pipeline for a given representation type
template <typename GraphRepresentation>
void execute_graph_pipeline(const std::string& repsentationName) {
    GraphRepresentation graph;

    // MST uses undirected graphs, SP and MF use directed graphs
    bool isDirected = (Parameters::problem != Parameters::Problems::mst);

    std::cout << "\n----------------------------------------\n";
    std::cout << " Processing Representation: " << repsentationName << "\n";
    std::cout << "----------------------------------------\n";
    std::cout << "[INFO] Loading graph from file: " << Parameters::inputFile << "\n";

    // attempt to load the file into the current representation template
    if (GraphParser::loadFromFile(Parameters::inputFile, graph, isDirected)) {
        
        // print loaded graph state for verification before running algorithms
        std::cout << "[INFO] Graph successfully loaded. Current representation state:";
        graph.print();

        std::cout << "\n[INFO] Running task: " << problem_to_string(Parameters::problem) << "\n";
        std::cout << "[INFO] Selected execution target: " << algorithm_to_string(Parameters::algorithm) << "\n";
        
        // run selected algorithm(s)
        bool success = run_selected_graph_algorithm<GraphRepresentation>(graph);
        
        if (success) {
            std::cout << "[SUCCESS] Finished processing pipeline for " << repsentationName << ".\n";
        } else {
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
    
    // check if parameters are properly set before proceeding
    if (Parameters::problem == Parameters::Problems::undefined) {
        std::cerr << "[ERROR] No problem type specified. Aborting single file mode.\n";
        return;
    }

    // handle execution based on selected graph structure, allowing for multiple representations if 'allStructures' is selected
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