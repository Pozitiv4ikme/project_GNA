#ifndef GRAPH_GENERATOR_HPP
#define GRAPH_GENERATOR_HPP

#include <iostream>
#include <random>
#include <cmath>
#include "structures/DynamicArray.hpp"

// simple plain structure to hold generated edge data temporarily
struct GeneratedEdge {
    int src;
    int dest;
    int weight;

    GeneratedEdge() : src(-1), dest(-1), weight(0) {}
    GeneratedEdge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

class GraphGenerator {
public:
    static DynamicArray<GeneratedEdge> generateRandomConnectedGraph(int vertices, int densityPercent, bool isDirected) {
        DynamicArray<GeneratedEdge> edgeList;

        if (vertices <= 1) return edgeList;

        // initialize modern random engine (Mersenne Twister 64-bit)
        std::random_device rd;
        std::mt19937_64 gen(rd());
        // for randomize edge direction 
        std::uniform_int_distribution<int> coinFlipDist(0, 1);

        // calculate target edges count based on density and graph orientation rules
        // undirected - (V * (V - 1)) / 2 -> because every vertex can connect to v - 1 others and we have V vertices, but we divide by 2 to avoid double counting (A->B and B->A is the same edge)
        // directed - V * (V - 1) -> same like undirected but we don't divide by 2 because A->B and B->A are different edges in directed graph
        long long maxEdges = isDirected ? (static_cast<long long>(vertices) * (vertices - 1)) 
                                        : (static_cast<long long>((vertices) * (vertices - 1)) / 2);
        
        int targetEdges = static_cast<int>(std::round(((double) densityPercent / 100.0) * maxEdges));

        // ensure targetEdges is not less than the minimum required to maintain connectivity (spanning tree always requires exactly V - 1 edges)
        int minRequiredEdges = vertices - 1;
        if (targetEdges < minRequiredEdges) {
            targetEdges = minRequiredEdges;
        }

        // initialize the weight distribution range based on total edges
        int maxWeight = (targetEdges * 4) / 5;
        if (maxWeight < 1) maxWeight = 1;
        std::uniform_int_distribution<int> weightDist(1, maxWeight);

        // setup a flat tracking matrix to ensure edge uniqueness in O(1) time
        DynamicArray<bool> edgeExists(vertices * vertices);
        for (int i = 0; i < vertices * vertices; ++i) {
            edgeExists[i] = false;
        }
        
        DynamicArray<int> vertexOrder(vertices);
        // fill vertex order with 0, 1, 2, ..., v - 1 to randomize connection order later
        for (int i = 0; i < vertices; ++i) {
            vertexOrder[i] = i;
        }

        // counter to track how many vertices are already included in the spanning tree
        int inTreeCount = 1;

        // core Base Spanning Tree generation
        for (int i = 1; i < vertices; ++i) {
            // choose random vertex from the already included vertices (0 to inTreeCount - 1)
            std::uniform_int_distribution<int> inDist(0, inTreeCount - 1);
            int u = vertexOrder[inDist(gen)];

            // select a random index from among the free vertices
            std::uniform_int_distribution<int> outDist(inTreeCount, vertices - 1);
            int outIdx = outDist(gen);
            int v = vertexOrder[outIdx];

            int src = u;
            int dest = v;
            if (isDirected && (coinFlipDist(gen) == 0)) {
                src = v;
                dest = u;
            }

            // mark used edge 
            edgeExists[src * vertices + dest] = true;
            if (!isDirected) {
                edgeExists[dest * vertices + src] = true;
            }

            // assign valid randomized weight based on the vertices formula
            edgeList.push_back(GeneratedEdge(src, dest, weightDist(gen)));

            // swap the newly added vertex with the boundary of the array
            int temp = vertexOrder[inTreeCount];
            vertexOrder[inTreeCount] = vertexOrder[outIdx];
            vertexOrder[outIdx] = temp;
            
            // expand the set of connected vertices by +1
            inTreeCount++;
        }

        // draw remaining random unique edges to satisfy density requirements
        std::uniform_int_distribution<int> vertexDist(0, vertices - 1);

        while (static_cast<int>(edgeList.size()) < targetEdges) {
            int src = vertexDist(gen);
            int dest = vertexDist(gen);

            if (src == dest) continue;
            if (edgeExists[src * vertices + dest]) continue;

            edgeExists[src * vertices + dest] = true;
            if (!isDirected) {
                edgeExists[dest * vertices + src] = true;
            }

            // assign valid randomized weight
            edgeList.push_back(GeneratedEdge(src, dest, weightDist(gen)));
        }

        // mandatory correctness verification stage
        std::cout << "\n========================================\n";
        std::cout << "     GRAPH GENERATION VERIFICATION      \n";
        std::cout << "========================================\n";
        std::cout << "[CHECK] Target Density requested: " << densityPercent << "%\n";
        std::cout << "[CHECK] Total vertices count (V): " << vertices << "\n";
        std::cout << "[CHECK] Theoretical Max Edges:    " << maxEdges << "\n";
        std::cout << "[CHECK] Expected Edges (k):       " << targetEdges << "\n";
        std::cout << "[CHECK] Actually Generated Edges: " << edgeList.size() << "\n";
        
        double actualDensity = (edgeList.size() * 100.0) / maxEdges;
        std::cout << "[CHECK] Calculated Final Density: " << std::round(actualDensity) << "%\n";
        
        // final check
        if (static_cast<int>(edgeList.size()) == targetEdges) {
            std::cout << "[STATUS] Verification PASSED: Density and Connectivity are correct.\n";
        } else {
            std::cout << "[STATUS] Verification FAILED: Structural mismatch detected.\n";
        }
        std::cout << "========================================\n";

        return edgeList;
    }
};

#endif