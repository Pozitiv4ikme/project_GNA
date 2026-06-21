#include <iostream>
#include "Parameters.h"
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"
#include "AlgorithmDispatcher.hpp"
#include "data/GraphGenerator.hpp"

namespace Parameters {
    RunModes runMode = RunModes::singleFile;
    Problems problem = Problems::sp;
    Structures structure = Structures::allStructures;
    std::string inputFile = "";
    std::string outputFile = ""; // keep empty so SingleFileMode skips writing file during this test
    std::string resultsFile = "";
    int vertexStart = 0;
    int vertexEnd = 0;
    int vertexCount = 0;
    int density = 0;
    int iterations = 1;
}

void test_directed_graph_generation() {
    std::cout << "\n>>> TEST 1: DIRECTED GRAPH GENERATION & SHORTEST PATH (DIJKSTRA) <<<\n";
    
    int vertices = 5;
    int densityPercent = 90;
    bool isDirected = false;

    // generate the random graph data instance
    DynamicArray<GeneratedEdge> rawEdges = GraphGenerator::generateRandomConnectedGraph(vertices, densityPercent, isDirected);

    // initialize and populate structures
    IncidenceMatrix matrix;
    matrix.initialize(vertices, rawEdges.size(), isDirected);
    SuccessorList list;
    list.initialize(vertices, rawEdges.size(), isDirected);

    for (size_t i = 0; i < rawEdges.size(); ++i) {
        matrix.addEdge(i, rawEdges[i].src, rawEdges[i].dest, rawEdges[i].weight);
        list.addEdge(i, rawEdges[i].src, rawEdges[i].dest, rawEdges[i].weight);
    }

    // print both representations
    matrix.print(std::cout);
    list.print(std::cout);

    // setup algorithm parameters
    Parameters::problem = Parameters::Problems::sp;
    Parameters::algorithm = Parameters::Algorithms::dijkstra;
    Parameters::vertexStart = 0;
    Parameters::vertexEnd = 4;

    // execute algorithm
    std::cout << "\n[RUN] Running Dijksta on Incidence Matrix:\n";
    run_selected_graph_algorithm(matrix, std::cout);

    std::cout << "\n[RUN] Running Dijkstra on Successor List:\n";
    run_selected_graph_algorithm(list, std::cout);
}

int main() {
    test_directed_graph_generation();
    
    return 0;
}