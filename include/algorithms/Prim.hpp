#ifndef PRIM_HPP
#define PRIM_HPP

#include <iostream>
#include <limits>
#include <type_traits>
#include "structures/DynamicArray.hpp"
#include "structures/MinHeap.hpp"
#include "representation/SuccessorList.hpp"

template <typename GraphRepresentation>
void run_prim_algorithm(const GraphRepresentation& graph, std::ostream& out = std::cout) {
    int V = graph.getVerticesCount();
    
    DynamicArray<int> parent(V);
    DynamicArray<int> key(V);
    DynamicArray<bool> inMST(V);

    // Initialize graph tracking arrays before running Prim's algorithm
    // - key[i]: stores the minimum edge weight to connect vertex i to the MST (initially infinity)
    // - inMST[i]: tracks whether vertex i is already included in the MST
    // - parent[i]: stores the parent of vertex i in the resulting MST tree
    for (int i = 0; i < V; ++i) {
        key[i] = std::numeric_limits<int>::max();
        inMST[i] = false;
        parent[i] = -1;
    }

    MinHeap priority_queue;
    
    // start with vertex 0
    key[0] = 0;
    priority_queue.push(0, 0);

    while (!priority_queue.empty()) {
        HeapNode topNode = priority_queue.top();
        priority_queue.pop();

        int u = topNode.vertex;

        // lazy deletion check: skip if this vertex duplicate was already processed into MST
        if (inMST[u]) continue;

        inMST[u] = true;

        // process neighbors based on representation type
        if constexpr (std::is_same_v<GraphRepresentation, SuccessorList>) {
            auto* current = graph.getNeighbors(u).getHead();
            while (current != nullptr) {
                int v = current->data.target;
                int weight = current->data.weight;

                if (!inMST[v] && weight < key[v]) {
                    parent[v] = u;
                    key[v] = weight;
                    priority_queue.push(v, key[v]); // push updated priority node
                }
                current = current->next;
            }
        } else {
            int edge_count = graph.getEdgesCount();
            for (int e = 0; e < edge_count; ++e) {
                if (graph.at(u, e) == 1) {
                    for (int v = 0; v < V; ++v) {
                        if (v != u && (graph.at(v, e) == -1 || graph.at(v, e) == 1)) {
                            int weight = graph.getEdgeWeight(e);
                            if (!inMST[v] && weight < key[v]) {
                                parent[v] = u;
                                key[v] = weight;
                                priority_queue.push(v, key[v]);
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
    out << "  MINIMUM SPANNING TREE (PRIM'S ALGO)   \n";
    out << "========================================\n";
    int total_mst_weight = 0;
    for (int i = 1; i < V; ++i) {
        if (parent[i] != -1) {
            out << "Edge: v" << parent[i] << " - v" << i << " | Weight: " << key[i] << "\n";
            total_mst_weight += key[i];
        }
    }
    out << "----------------------------------------\n";
    out << "Total MST Weight: " << total_mst_weight << "\n";
    out << "========================================\n";
}

#endif