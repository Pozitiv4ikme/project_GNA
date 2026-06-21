#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include <iostream>
#include <type_traits>
#include "structures/DynamicArray.hpp"
#include "representation/SuccessorList.hpp"
#include "representation/IncidenceMatrix.hpp"

/* 
helper structure to temporarily collect edges, because Kruskal's algorithm is not affected 
by the structure of the graph; it simply needs a list of all the edges in order to sort them
*/
struct PlainEdge {
    int src;
    int dest;
    int weight;
    PlainEdge() : src(-1), dest(-1), weight(0) {}
    PlainEdge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

// DSU helper functions for quick cycle detection
inline int dsu_find(int i, const DynamicArray<int>& parent) {
    while (parent[i] != i) {
        i = parent[i];
    }
    return i;
}

inline void dsu_union(int i, int j, DynamicArray<int>& parent) {
    int root_i = dsu_find(i, parent);
    int root_j = dsu_find(j, parent);
    parent[root_i] = root_j;
}

template <typename GraphRepresentation>
void run_kruskal_algorithm(const GraphRepresentation& graph, std::ostream& out = std::cout) {
    int V = graph.getVerticesCount();
    int E = graph.getEdgesCount();

    DynamicArray<PlainEdge> all_edges;

    // collect all unique edges from the graph representation
    if constexpr (std::is_same_v<GraphRepresentation, SuccessorList>) {
        for (int u = 0; u < V; ++u) {
            auto* current = graph.getNeighbors(u).getHead();
            while (current != nullptr) {
                // to avoid duplication in undirected graph, only take u < target
                if (u < current->data.target) {
                    all_edges.push_back(PlainEdge(u, current->data.target, current->data.weight));
                }
                current = current->next;
            }
        }
    } else {
        for (int e = 0; e < E; ++e) {
            int src = -1, dest = -1;
            // for undirected matrix, both endpoints are marked with 1
            for (int v = 0; v < V; ++v) {
                if (graph.at(v, e) == 1) {
                    if (src == -1) src = v;
                    else { dest = v; break; }
                }
            }
            if (src != -1 && dest != -1) {
                all_edges.push_back(PlainEdge(src, dest, graph.getEdgeWeight(e)));
            }
        }
    }

    // sort edges by weight using a simple Insertion Sort (with strict tie-breaking)
    for (size_t i = 1; i < all_edges.size(); ++i) {
        PlainEdge key = all_edges[i];
        int j = static_cast<int>(i) - 1;
        
        // helper rule: if the weights are equal, compare the vertex key (numbers)
        auto should_swap = [](const PlainEdge& a, const PlainEdge& b) {
            if (a.weight != b.weight) {
                return a.weight > b.weight;
            }
            if (a.src != b.src) {
                return a.src > b.src;
            }
            return a.dest > b.dest;
        };

        while (j >= 0 && should_swap(all_edges[static_cast<size_t>(j)], key)) {
            all_edges[static_cast<size_t>(j + 1)] = all_edges[static_cast<size_t>(j)];
            j--;
        }
        all_edges[static_cast<size_t>(j + 1)] = key;
    }

    // initialize DSU tracking arrays
    DynamicArray<int> dsu_parent(V);
    for (int i = 0; i < V; ++i) {
        dsu_parent[i] = i;
    }

    DynamicArray<PlainEdge> mst_edges;
    int total_mst_weight = 0;

    // build MST by picking the smallest non-cyclic edges
    for (size_t i = 0; i < all_edges.size(); ++i) {
        PlainEdge current_edge = all_edges[i];
        
        int root_src = dsu_find(current_edge.src, dsu_parent);
        int root_dest = dsu_find(current_edge.dest, dsu_parent);

        // if roots are different, it means they are not in the same set (no cycle)
        if (root_src != root_dest) {
            mst_edges.push_back(current_edge);
            total_mst_weight += current_edge.weight;
            dsu_union(root_src, root_dest, dsu_parent);
        }

        // MST always contains exactly V - 1 edges
        if (mst_edges.size() == static_cast<size_t>(V - 1)) break;
    }

    // report printing
    out << "\n========================================\n";
    out << "  MINIMUM SPANNING TREE (KRUSKAL'S ALGO)\n";
    out << "========================================\n";
    for (size_t i = 0; i < mst_edges.size(); ++i) {
        out << "Edge: v" << mst_edges[i].src << " - v" << mst_edges[i].dest 
            << " | Weight: " << mst_edges[i].weight << "\n";
    }
    out << "----------------------------------------\n";
    out << "Total MST Weight: " << total_mst_weight << "\n";
    out << "========================================\n";
}

#endif