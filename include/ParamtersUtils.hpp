#ifndef PARAMETERSUTILS_HPP
#define PARAMETERSUTILS_HPP

#include <string>
#include "Parameters.h"

// Convert Problems enum to string for logging and console output
std::string problem_to_string(Parameters::Problems p) {
    switch(p) {
        case Parameters::Problems::mst: return "Minimum Spanning Tree (MST)";
        case Parameters::Problems::sp:  return "Shortest Path (SP)";
        case Parameters::Problems::mf:  return "Maximum Flow (MF)";
        default:                        return "Undefined Problem";
    }
}

// Convert Algorithms enum to string for logging and CSV outputs
std::string algorithm_to_string(Parameters::Algorithms a) {
    switch(a) {
        case Parameters::Algorithms::allAlgorithms: return "All Algorithms";
        case Parameters::Algorithms::prim:          return "Prim's Algorithm";
        case Parameters::Algorithms::kruskal:       return "Kruskal's Algorithm";
        case Parameters::Algorithms::dijkstra:      return "Dijkstra's Algorithm";
        case Parameters::Algorithms::bellmanFord:   return "Bellman-Ford Algorithm";
        case Parameters::Algorithms::fordFulkerson: return "Ford-Fulkerson Algorithm";
        default:                                    return "Undefined Algorithm";
    }
}

// Convert Structures enum to string
std::string structure_to_string(Parameters::Structures s) {
    switch(s) {
        case Parameters::Structures::allStructures:  return "All Representations";
        case Parameters::Structures::incidenceMatrix: return "Incidence Matrix";
        case Parameters::Structures::adjacencyList:   return "Adjacency List (Successor)";
        default:                                      return "Undefined Structure";
    }
}

#endif