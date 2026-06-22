#ifndef BENCHMARKMODE_HPP
#define BENCHMARKMODE_HPP

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>
#include "Parameters.h"
#include "ParamtersUtils.hpp"
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"
#include "data/GraphGenerator.hpp"
#include "AlgorithmDispatcher.hpp"

// helper function to get current date and time for CSV logging
inline std::string get_benchmark_timestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// universal template executor for benchmarking a specific graph representation
template <typename GraphRepresentation>
void execute_benchmark_pipeline(const std::string& representationName, Parameters::Algorithms specificAlgorithm) {
    // temporarily swap global parameter if we are iterating through "All Algorithms"
    Parameters::Algorithms backupAlgorithm = Parameters::algorithm;
    Parameters::algorithm = specificAlgorithm;

    std::ofstream csvFile(Parameters::resultsFile, std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "[ERROR] Benchmark cannot open results CSV file: " << Parameters::resultsFile << "\n";
        Parameters::algorithm = backupAlgorithm;
        return;
    }

    // write CSV header if the file is freshly created and empty
    csvFile.seekp(0, std::ios::end);
    if (csvFile.tellp() == 0) {
        csvFile << "Date_Time;Problem;Algorithm;Structure;Vertices;Density;Iteration;Time_us\n";
    }

    std::cout << "[BENCHMARK] Starting loop: " << algorithm_to_string(specificAlgorithm) 
              << " on " << representationName << " (" << Parameters::iterations << " iterations)...\n";

    // automatically inject standard Source and Sink vertices for SP and MF tasks
    if (Parameters::problem == Parameters::Problems::sp || Parameters::problem == Parameters::Problems::mf) {
        Parameters::vertexStart = 0;
        Parameters::vertexEnd = Parameters::vertexCount - 1;
    }

    double totalDuration = 0;
    long long minDuration = 9223372036854775807LL; // Max long long upper bound
    long long maxDuration = -1;

    bool isDirected = (Parameters::problem != Parameters::Problems::mst);

    // core statistical gathering iteration loop
    for (int i = 0; i < Parameters::iterations; ++i) {
        // generate a brand new random connected graph for this iteration
        auto rawEdges = GraphGenerator::generateRandomConnectedGraph(Parameters::vertexCount, Parameters::density, isDirected);

        GraphRepresentation graph;
        graph.initialize(Parameters::vertexCount, rawEdges.size(), isDirected);
        for (size_t e = 0; e < rawEdges.size(); ++e) {
            graph.addEdge(static_cast<int>(e), rawEdges[e].src, rawEdges[e].dest, rawEdges[e].weight);
        }

        // create a dummy stringstream to absorb all console prints (crucial for pure time benchmarks)
        std::stringstream discardStream;

        // time measurement block using high resolution steady clock
        auto startTime = std::chrono::high_resolution_clock::now();
        
        bool success = run_selected_graph_algorithm<GraphRepresentation>(graph, discardStream);
        
        auto endTime = std::chrono::high_resolution_clock::now();

        if (!success) {
            std::cerr << "[ERROR] Algorithm failed execution during benchmark iteration " << i << "\n";
            continue;
        }

        // calculate precise duration in microseconds
        auto elapsedMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        // accumulate statistics
        totalDuration += elapsedMicros;
        if (elapsedMicros < minDuration) minDuration = elapsedMicros;
        if (elapsedMicros > maxDuration) maxDuration = elapsedMicros;

        // append this raw single iteration metric line into the CSV spreadsheet
        csvFile << get_benchmark_timestamp() << ";"
                << problem_to_string(Parameters::problem) << ";"
                << algorithm_to_string(specificAlgorithm) << ";"
                << representationName << ";"
                << Parameters::vertexCount << ";"
                << Parameters::density << ";"
                << i << ";"
                << elapsedMicros << "\n";
    }

    csvFile.close();

    // log compiled average results to console for immediate visibility
    double avgDuration = totalDuration / Parameters::iterations;
    std::cout << "[RESULT] " << algorithm_to_string(specificAlgorithm) << " (" << representationName << ") -> "
              << "Avg: " << std::fixed << std::setprecision(2) << avgDuration << " us | "
              << "Min: " << minDuration << " us | "
              << "Max: " << maxDuration << " us\n";

    // restore original parameter backup state
    Parameters::algorithm = backupAlgorithm;
}

// router to handle selected structures
template <typename GraphRepresentation>
void route_benchmark_structures(const std::string& representationName) {
    if (Parameters::algorithm == Parameters::Algorithms::allAlgorithms) {
        if (Parameters::problem == Parameters::Problems::mst) {
            execute_benchmark_pipeline<GraphRepresentation>(representationName, Parameters::Algorithms::prim);
            execute_benchmark_pipeline<GraphRepresentation>(representationName, Parameters::Algorithms::kruskal);
        } 
        else if (Parameters::problem == Parameters::Problems::sp) {
            execute_benchmark_pipeline<GraphRepresentation>(representationName, Parameters::Algorithms::dijkstra);
            execute_benchmark_pipeline<GraphRepresentation>(representationName, Parameters::Algorithms::bellmanFord);
        } 
        else if (Parameters::problem == Parameters::Problems::mf) {
            execute_benchmark_pipeline<GraphRepresentation>(representationName, Parameters::Algorithms::fordFulkerson);
        }
    } else {
        // execute only the single explicitly requested algorithm target
        execute_benchmark_pipeline<GraphRepresentation>(representationName, Parameters::algorithm);
    }
}

// main execution entry point dispatcher for Benchmark Research Mode
void run_benchmark_mode() {
    std::cout << "\n========================================\n";
    std::cout << "        BENCHMARK MODE ACTIVATED        \n";
    std::cout << "========================================\n";

    if (Parameters::problem == Parameters::Problems::undefined) {
        std::cerr << "[ERROR] No problem type specified. Aborting benchmark runs.\n";
        return;
    }
    if (Parameters::resultsFile.empty()) {
        std::cerr << "[ERROR] No CSV output target results file specified.\n";
        return;
    }

    // dispatch based on structure configuration parameters
    if (Parameters::structure == Parameters::Structures::incidenceMatrix) {
        route_benchmark_structures<IncidenceMatrix>("Incidence_Matrix");
    } 
    else if (Parameters::structure == Parameters::Structures::adjacencyList) {
        route_benchmark_structures<SuccessorList>("Successor_List");
    } 
    else if (Parameters::structure == Parameters::Structures::allStructures) {
        std::cout << "[INFO] Option 'All Structures' selected. Benchmarking both sequentially.\n";
        route_benchmark_structures<IncidenceMatrix>("Incidence_Matrix");
        route_benchmark_structures<SuccessorList>("Successor_List");
    } 
    else {
        std::cout << "[WARNING] Selected benchmark graph structure layout is unsupported!\n";
    }

    std::cout << "========================================\n";
    std::cout << "       FINISHED BENCHMARK MODE          \n";
    std::cout << "========================================\n";
}

#endif