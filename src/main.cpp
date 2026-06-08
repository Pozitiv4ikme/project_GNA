#include <iostream>
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"

int main() {
    std::cout << "Testing Graph Representations...\n";

    int vertices = 4;
    int edges = 3;
    bool isDirected = true; // Let's test a directed graph first

    // 1. Test Matrix
    IncidenceMatrix matrix;
    matrix.initialize(vertices, edges, isDirected);
    matrix.addEdge(0, 0, 1, 10); // e0: v0 -> v1 (weight 10)
    matrix.addEdge(1, 1, 2, 20); // e1: v1 -> v2 (weight 20)
    matrix.addEdge(2, 2, 3, 30); // e2: v2 -> v3 (weight 30)
    matrix.print();

    // 2. Test List
    SuccessorList list;
    list.initialize(vertices, edges, isDirected);
    list.addEdge(0, 0, 1, 10);
    list.addEdge(1, 1, 2, 20);
    list.addEdge(2, 2, 3, 30);
    list.print();

    return 0;
}