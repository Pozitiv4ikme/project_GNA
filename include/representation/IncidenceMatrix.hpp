#ifndef INCIDENCE_MATRIX_HPP
#define INCIDENCE_MATRIX_HPP

#include <iostream>
#include <iomanip>
#include "structures/DynamicArray.hpp"

class IncidenceMatrix {
private:
    DynamicArray<int> main_matrix;  // 1D array simulating 2D matrix of size V * E 
    DynamicArray<int> additional_weights_arr; // Array of size E to store weights of edges
    
    int n_vertices;
    int n_edges;
    bool isDirected;

public:
    IncidenceMatrix() : n_vertices(0), n_edges(0), isDirected(false) {}

    // helper function to access matrix elements to change it or read it
    int& at(int v, int e) {
        // v - vertex index that we want to access, e - edge index, n_edges - number of edges in the graph
        // so we choose column v and row e and return info about connection between them (1 for outgoing, -1 for incoming, 0 for no connection)
        return main_matrix[v * n_edges + e];
    }

    // const version of the at() for read-only access
    int at(int v, int e) const {
        return main_matrix[v * n_edges + e];
    }

    // Allocates memory for the matrix based on file data
    void initialize(int vertices, int edges, bool directed) {
        n_vertices = vertices;
        n_edges = edges;
        isDirected = directed;
        
        // Resize arrays using DynamicArray implementation 
        main_matrix = DynamicArray<int>(vertices * edges);
        additional_weights_arr = DynamicArray<int>(edges);

        // Fill matrix with zeros
        for (int i = 0; i < vertices * edges; ++i) {
            main_matrix[i] = 0;
        }
        for (int i = 0; i < edges; ++i) {
            additional_weights_arr[i] = 0;
        }
    }

    // Add edge into the matrix
    void addEdge(int edgeIdx, int src, int dest, int weight) {
        // validation of input data (checking boundaries)
        if (edgeIdx >= n_edges || src >= n_vertices || dest >= n_vertices) return;

        // add edge by it index and set its weight in the additional weights array
        additional_weights_arr[edgeIdx] = weight;

        if (isDirected) {
            // 1 for outgoing (source), -1 for incoming (destination)
            at(src, edgeIdx) = 1;
            at(dest, edgeIdx) = -1;
        } else {
            // Undirected edge connects both vertices equally
            at(src, edgeIdx) = 1;
            at(src, edgeIdx) = 1;
        }
    }

    int getVerticesCount() const { return n_vertices; }
    int getEdgesCount() const { return n_edges; }

    // Prints the matrix to console
    void print() const {
        const int cellWidth = 4; // width for each cell in the output
        std::cout << "\n--- Incidence Matrix ---\n";

        std::cout << std::setw(10) << "Vert\\Edge";
        for (int e = 0; e < n_edges; ++e) {
            std::cout << std::setw(cellWidth) << "e" + std::to_string(e);
        }
        std::cout << "\n";

        for (int v = 0; v < n_vertices; ++v) {
            std::cout << std::setw(10) << "v" + std::to_string(v);
            for (int e = 0; e < n_edges; ++e) {
                std::cout << std::setw(cellWidth) << at(v, e);
            }
            std::cout << "\n";
        }

        std::cout << std::setw(10) << "Weights:";
        for (int e = 0; e < n_edges; ++e) {
            std::cout << std::setw(cellWidth) << additional_weights_arr[e];
        }
        std::cout << "\n";
    }
};

#endif // INCIDENCE_MATRIX_HPP