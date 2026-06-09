#ifndef BELLMAN_FORD_HPP
#define BELLMAN_FORD_HPP

#include <iostream>
#include "structures/DynamicArray.hpp"

template <typename GraphRepresentation>
void run_bellman_ford_algorithm(const GraphRepresentation& graph, int startVertex, int endVertex, std::ostream& out = std::cout) {
    int V = graph.getVerticesCount();
    int E = graph.getEdgesCount();

    // save current shortest distances from source to each vertex
    DynamicArray<int> dist(V);

    // save parent vertex (index) for each vertex to reconstruct the path later
    DynamicArray<int> parent(V);

    // initialize distances: source is 0, all other vertices are infinity
    for (int i = 0; i < V; ++i) {
        dist[i] = 2147483647; // Standard INF representation (max int value)
        parent[i] = -1;
    }
    dist[startVertex] = 0;

    // core logic: relax all edges exactly V - 1 times
    for (int i = 1; i <= V - 1; ++i) {
        
        if constexpr (std::is_same_v<GraphRepresentation, SuccessorList>) {
            // for SuccessorList: iterate through all adjacency lists
            for (int s = 0; s < V; ++s) {
                auto* current = graph.getNeighbors(s).getHead();
                while (current != nullptr) {
                    int v = current->data.target;
                    int weight = current->data.weight;
                    
                    if (dist[s] != 2147483647 && dist[s] + weight < dist[v]) {
                        dist[v] = dist[s] + weight;
                        parent[v] = s;
                    }
                    current = current->next;
                }
            }
        } else {
            // for IncidenceMatrix: iterate through all columns (edges)
            for (int e = 0; e < E; ++e) {
                int src = -1, dest = -1;

                // find source (1) and destination (-1) for directed edge 'e'
                for (int v = 0; v < V; ++v) {
                    if (graph.at(v, e) == 1) src = v;
                    else if (graph.at(v, e) == -1) dest = v;
                }
                
                if (src != -1 && dest != -1) {
                    int weight = graph.getEdgeWeight(e);
                    if (dist[src] != 2147483647 && dist[src] + weight < dist[dest]) {
                        dist[dest] = dist[src] + weight;
                        parent[dest] = src;
                    }
                }
            }
        }
    }

    // clearly print results to console in a user-friendly format
    out << "\n========================================\n";
    out << "    SHORTEST PATH (BELLMAN-FORD ALGO)   \n";
    out << "========================================\n";
    out << "Source: v" << startVertex << " -> Destination: v" << endVertex << "\n";
    out << "----------------------------------------\n";

    if (dist[endVertex] == 2147483647) {
        out << "Path: No connection found between v" << startVertex << " and v" << endVertex << "\n";
    } else {
        out << "Total Path Cost: " << dist[endVertex] << "\n";

        // reconstruct path backward using custom DynamicArray
        DynamicArray<int> path;
        int current = endVertex;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }

        out << "Path Taken: ";
        for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
            out << "v" << path[i];

            // will not print arrow after last vertex in the path
            if (i > 0) out << " -> ";
        }
        out << "\n";
    }
    out << "========================================\n";
};

#endif