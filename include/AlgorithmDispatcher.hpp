#ifndef ALGORITHM_DISPATCHER_HPP
#define ALGORITHM_DISPATCHER_HPP

#include <iostream>
#include "Parameters.h"
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"

#include "algorithms/BellmanFord.hpp"
#include "algorithms/Prim.hpp"

template <typename GraphRepresentation>
bool run_selected_graph_algorithm(GraphRepresentation& graph, std::ostream& out = std::cout) {
    if (Parameters::problem == Parameters::Problems::mst) {
        if (Parameters::algorithm == Parameters::Algorithms::prim || 
            Parameters::algorithm == Parameters::Algorithms::allAlgorithms) {
            run_prim_algorithm(graph, out);
            return true;
        }
    } 
    else if (Parameters::problem == Parameters::Problems::sp) {
        if (Parameters::algorithm == Parameters::Algorithms::bellmanFord || 
            Parameters::algorithm == Parameters::Algorithms::allAlgorithms) {
            run_bellman_ford_algorithm(graph, Parameters::vertexStart, Parameters::vertexEnd, out);
            return true;
        }
    }
    
    std::cout << "[WARNING] The requested configuration is not available in this build.\n";
    return false;
}

#endif