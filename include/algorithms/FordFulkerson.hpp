#ifndef FORD_FULKERSON_HPP
#define FORD_FULKERSON_HPP

#include <iostream>
#include <type_traits>
#include "structures/DynamicArray.hpp"
#include "representation/SuccessorList.hpp"
#include "representation/IncidenceMatrix.hpp"

// BFS helper to find an augmenting path in the residual graph
inline bool ford_fulkerson_bfs(int start, int end, int V, const DynamicArray<int>& residualCapacity, DynamicArray<int>& parent) {
    DynamicArray<bool> visited(V);
    for (int i = 0; i < V; ++i) {
        visited[i] = false;
        parent[i] = -1;
    }

    // standard queue behavior simulated using a custom DynamicArray tracker
    DynamicArray<int> queue;
    queue.push_back(start);
    visited[start] = true;

    size_t head = 0;
    while (head < queue.size()) {
        int u = queue[head++];

        for (int v = 0; v < V; ++v) {
            // if not visited and there is available residual capacity left
            if (!visited[v] && residualCapacity[static_cast<size_t>(u * V + v)] > 0) {
                parent[static_cast<size_t>(v)] = u;
                visited[static_cast<size_t>(v)] = true;
                queue.push_back(v);
                
                if (v == end) return true; // augmented path found early checkout
            }
        }
    }
    return visited[static_cast<size_t>(end)];
}

template <typename GraphRepresentation>
void run_ford_fulkerson_algorithm(const GraphRepresentation& graph, int startVertex, int endVertex, std::ostream& out = std::cout) {
    int V = graph.getVerticesCount();
    
    // create a flat 2D residual capacity matrix initialized to 0
    DynamicArray<int> residualCapacity(V * V);
    for (int i = 0; i < V * V; ++i) {
        residualCapacity[i] = 0;
    }

    // copy initial edge capacities from selected graph representation to residual matrix
    if constexpr (std::is_same_v<GraphRepresentation, SuccessorList>) {
        for (int u = 0; u < V; ++u) {
            auto* current = graph.getNeighbors(u).getHead();
            while (current != nullptr) {
                int v = current->data.target;
                int weight = current->data.weight;
                residualCapacity[static_cast<size_t>(u * V + v)] = weight;
                current = current->next;
            }
        }
    } else {
        int E = graph.getEdgesCount();
        for (int e = 0; e < E; ++e) {
            int src = -1, dest = -1;
            bool is_edge_directed = false;

            for (int v = 0; v < V; ++v) {
                int value = graph.at(v, e);
                if (value == 1) {
                    if (src == -1) {
                        src = v; // first vertex found
                    } else {
                        dest = v; // second vertex found (meaning the graph is undirected)
                    }
                } else if (value == -1) {
                    dest = v; // minus one is the exact end of an oriented edge
                    is_edge_directed = true;
                }
            }

            if (src != -1 && dest != -1) {
                int weight = graph.getEdgeWeight(e);
                
                // record the direct throughput
                residualCapacity[static_cast<size_t>(src * V + dest)] = weight;
                
                // if the graph is undirected, the flow can also flow in the opposite direction with the same weight
                if (!is_edge_directed) {
                    residualCapacity[static_cast<size_t>(dest * V + src)] = weight;
                }
            }
        }
    }

    DynamicArray<int> parent(V);
    int max_flow = 0;

    // augment the flow while a path from source to sink exists
    while (ford_fulkerson_bfs(startVertex, endVertex, V, residualCapacity, parent)) {
        
        // find the bottleneck (minimum residual capacity) along the path
        int path_flow = INF;
        // follow the path we found in reverse: from the end (endVertex) to the start (startVertex)
        for (int v = endVertex; v != startVertex; v = parent[static_cast<size_t>(v)]) {
            int u = parent[static_cast<size_t>(v)]; // get vertex u from which we arrived at v
            int capacity = residualCapacity[static_cast<size_t>(u * V + v)]; // check its remaining capacity

            // find the minimum throughput along the entire path
            // this will determine the maximum amount of traffic we can process
            if (capacity < path_flow) {
                path_flow = capacity;
            }
        }

        // going back along the same path, from the outlet to the source
        for (int v = endVertex; v != startVertex; v = parent[static_cast<size_t>(v)]) {
            int u = parent[static_cast<size_t>(v)]; // get parent

            // reduce the forward bandwidth (we've taken up this space with a stream)
            residualCapacity[static_cast<size_t>(u * V + v)] -= path_flow;

            // increase the throughput in the reverse direction (add the ability to “undo” this flow later)
            residualCapacity[static_cast<size_t>(v * V + u)] += path_flow;
        }

        // add the found flow of the current path to the total maximum flow
        max_flow += path_flow;
    }

    // report printing
    out << "\n========================================\n";
    out << "     MAXIMUM FLOW (FORD-FULKERSON)      \n";
    out << "========================================\n";
    out << "Source Vertex: v" << startVertex << " -> Sink Vertex: v" << endVertex << "\n";
    out << "----------------------------------------\n";
    out << "Total Maximum Flow capacity: " << max_flow << "\n";
    out << "========================================\n";
}

#endif