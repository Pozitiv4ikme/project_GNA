#include <iostream>
#include "Parameters.h"
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"
#include "AlgorithmDispatcher.hpp"
#include "data/GraphGenerator.hpp" // Adjust path if your generator is in another folder

// Define external variables from Parameters.h to satisfy the linker
// (Normally initialized by the teacher's library, but we mock them here for testing)
namespace Parameters {
    RunModes runMode = RunModes::singleFile;
    Problems problem = Problems::mst;
    Structures structure = Structures::allStructures;
    std::string inputFile = "";
    std::string outputFile = ""; // Keep empty so SingleFileMode skips writing file during this test
    std::string resultsFile = "";
    int vertexStart = 0;
    int vertexEnd = 0;
    int vertexCount = 0;
    int density = 0;
    int iterations = 1;
}

void test_directed_graph_generation() {
    std::cout << "\n>>> TEST 2: DIRECTED GRAPH GENERATION & SHORTEST PATH (BELLMAN-FORD) <<<\n";
    
    int vertices = 5;
    int densityPercent = 75; // 75% density target
    bool isDirected = true;   // Directed graph for SP

    // 1. Generate the random graph data instance
    DynamicArray<GeneratedEdge> rawEdges = GraphGenerator::generateRandomConnectedGraph(vertices, densityPercent, isDirected);

    // 2. Initialize and populate structures
    IncidenceMatrix matrix;
    matrix.initialize(vertices, rawEdges.size(), isDirected);
    SuccessorList list;
    list.initialize(vertices, rawEdges.size(), isDirected);

    for (size_t i = 0; i < rawEdges.size(); ++i) {
        matrix.addEdge(i, rawEdges[i].src, rawEdges[i].dest, rawEdges[i].weight);
        list.addEdge(i, rawEdges[i].src, rawEdges[i].dest, rawEdges[i].weight);
    }

    // 3. Print both representations
    matrix.print(std::cout);
    list.print(std::cout);

    // 4. Setup algorithm parameters (Path from vertex 0 to vertex 4)
    Parameters::problem = Parameters::Problems::sp;
    Parameters::algorithm = Parameters::Algorithms::bellmanFord;
    Parameters::vertexStart = 0;
    Parameters::vertexEnd = 4;

    // 5. Execute algorithm
    std::cout << "\n[RUN] Running Bellman-Ford on Incidence Matrix:\n";
    run_selected_graph_algorithm(matrix, std::cout);

    std::cout << "\n[RUN] Running Bellman-Ford on Successor List:\n";
    run_selected_graph_algorithm(list, std::cout);
}

int main() {
    // Run both test scenarios sequentially
    test_directed_graph_generation();
    
    return 0;
}