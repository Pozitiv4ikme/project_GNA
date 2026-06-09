#ifndef SUCCESSOR_LIST_HPP
#define SUCCESSOR_LIST_HPP

#include <iostream>
#include "structures/DynamicArray.hpp"
#include "structures/SingleList.hpp"
#include "../GraphEdge.hpp"

class SuccessorList {
    private:
        // list of successors for each vertex, where each successor is represented as a GraphEdge (target vertex and weight)
        DynamicArray<SingleList<GraphEdge>> successors;

        // for DynamicArray capacity
        int number_of_vertices;
        // numbers of edges for each vertex
        int number_of_edges;
        int is_directed;

    public:
        SuccessorList(): number_of_vertices(0), number_of_edges(0), is_directed(0) {}

        void initialize(int vertices, int edges, int directed) {
            number_of_vertices = vertices;
            number_of_edges = edges;
            is_directed = directed;

            successors = DynamicArray<SingleList<GraphEdge>>(vertices);
            for (int i = 0; i < vertices; ++i) {
                successors[i].clear();
            }
        }

        /* because this project using templates and we have 2 different representations of graphs, 
        we need to use here edgeIdx like parameter but not use for anything, like dummy parameter
        */
        void addEdge([[maybe_unused]] int edgeIdx, int src, int dest, int weight) {
            if (src >= number_of_vertices || dest >= number_of_vertices) return;

            successors[src].push_back(GraphEdge(dest, weight));
            if (!is_directed) {
                successors[dest].push_back(GraphEdge(src, weight));
            }
        }

        // Getter for custom iteration in algorithms
        const SingleList<GraphEdge>& getNeighbors(int vertex) const {
            return successors[vertex];
        }

        int getVerticesCount() const { return number_of_vertices; }
        int getEdgesCount() const { return number_of_edges; }

        void print() const {
            std::cout << "\n========================================\n";
            std::cout << "  SUCCESSOR LIST REPRESENTATION\n";
            std::cout << "  Type: " << (is_directed ? "Directed" : "Undirected") << "\n";
            std::cout << "  Vertices: " << number_of_vertices << ", Edges: " << number_of_edges << "\n";
            std::cout << "========================================\n";

            for (int v = 0; v < number_of_vertices; ++v) {
                std::cout << "v" << v << " ---> ";
                auto* current = successors[v].getHead();
                while (current != nullptr) {
                    std::cout << "[Target: v" << current->data.target << ", W: " << current->data.weight << "] -> ";
                    current = current->next;
                }
                std::cout << "NULL\n";
            }
            std::cout << "========================================\n";
        }
};

#endif