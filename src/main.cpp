#include <iostream>
#include "representation/IncidenceMatrix.hpp"
#include "representation/SuccessorList.hpp"
#include "data/GraphParser.hpp"

int main() {
    std::cout << "Testing Graph Parser...\n";

    std::string testFile = "input.txt";
    bool isDirected = false;

    // Test matrix loading
    IncidenceMatrix matrix;
    if (GraphParser::loadFromFile(testFile, matrix, isDirected)) {
        matrix.print();
    }

    // Test list loading
    SuccessorList list;
    if (GraphParser::loadFromFile(testFile, list, isDirected)) {
        list.print();
    }

    return 0;
}