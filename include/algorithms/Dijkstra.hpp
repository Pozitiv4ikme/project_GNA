#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <iostream>
#include <type_traits>
#include "structures/DynamicArray.hpp"
#include "structures/MinHeap.hpp"
#include "representation/SuccessorList.hpp"

static constexpr int INF = 2147483647;

template <typename GraphRepresentation>
void run_dijkstra_algorithm(const GraphRepresentation& graph, int startVertex, int endVertex, std::ostream& out = std::cout) {
    int V = graph.getVerticesCount();
    
    DynamicArray<int> shortest_path_cost(V);
    DynamicArray<int> parent(V);
    DynamicArray<bool> visited(V);

    for (int i = 0; i < V; ++i) {
        shortest_path_cost[i] = INF; 
        visited[i] = false;
        parent[i] = -1;
    }

    MinHeap priority_queue;

    shortest_path_cost[startVertex] = 0;
    priority_queue.push(startVertex, 0);

    while (!priority_queue.empty()) {
        HeapNode topNode = priority_queue.top();
        priority_queue.pop();

        int u = topNode.vertex;

        // lazy deletion validation check
        if (visited[u]) continue;
        visited[u] = true;

        if (u == endVertex) break; // optimization path found breakout

        if constexpr (std::is_same_v<GraphRepresentation, SuccessorList>) {
            auto* current = graph.getNeighbors(u).getHead();
            while (current != nullptr) {
                int v = current->data.target;
                int weight = current->data.weight;

                if (!visited[v] && shortest_path_cost[u] != INF && shortest_path_cost[u] + weight < shortest_path_cost[v]) {
                    parent[v] = u;
                    shortest_path_cost[v] = shortest_path_cost[u] + weight;
                    priority_queue.push(v, shortest_path_cost[v]);
                }
                current = current->next;
            }
        } else {
            int e_count = graph.getEdgesCount();
            for (int e = 0; e < e_count; ++e) {
                if (graph.at(u, e) == 1) {
                    for (int v = 0; v < V; ++v) {
                        if (v != u && (graph.at(v, e) == -1 || graph.at(v, e) == 1)) {
                            int weight = graph.getEdgeWeight(e);
                            if (!visited[v] && shortest_path_cost[u] != INF && shortest_path_cost[u] + weight < shortest_path_cost[v]) {
                                parent[v] = u;
                                shortest_path_cost[v] = shortest_path_cost[u] + weight;
                                priority_queue.push(v, shortest_path_cost[v]);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    // report printing
    out << "\n========================================\n";
    out << "    SHORTEST PATH (DIJKSTRA'S ALGO)     \n";
    out << "========================================\n";
    out << "Source: v" << startVertex << " -> Destination: v" << endVertex << "\n";
    out << "----------------------------------------\n";
    
    if (shortest_path_cost[endVertex] == INF) {
        out << "Path: No connection found between v" << startVertex << " and v" << endVertex << "\n";
    } else {
        out << "Total Path Cost: " << shortest_path_cost[endVertex] << "\n";
        
        DynamicArray<int> path;
        int current = endVertex;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        
        out << "Path Taken: ";
        for (int i = static_cast<int>(path.size()) - 1; i >= 0; --i) {
            out << "v" << path[i];
            if (i > 0) out << " -> ";
        }
        out << "\n";
    }
    out << "========================================\n";
}

#endif