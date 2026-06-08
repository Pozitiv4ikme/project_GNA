#ifndef GRAPH_PARSER_HPP
#define GRAPH_PARSER_HPP

#include <fstream>
#include <iostream>
#include <string>

class GraphParser {
public:
    // placeholder for ANY graph representation having initialize() and addEdge()
    template <typename GraphRepresentation> 
    static bool loadFromFile(const std::string& filename, GraphRepresentation& graph, bool isDirected) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << "\n";
            return false;
        }

        int vertices = 0;
        int edges = 0;

        // Read the first line: total vertices and edges count
        // >> skips any whitespace characters, so it work with ' ', '\t' and '\n' as separators
        if (!(file >> vertices >> edges)) {
            std::cerr << "Error: Invalid file header format.\n";
            return false;
        }

        graph.initialize(vertices, edges, isDirected);

        int src = 0, dest = 0, weight = 0;
        // Read subsequent lines containing edge data
        for (int i = 0; i < edges; ++i) {
            if (file >> src >> dest >> weight) {
                graph.addEdge(i, src, dest, weight);
            } else {
                std::cerr << "Warning: Reached EOF or bad data before reading all declared edges.\n";
                return false;
            }
        }

        file.close();
        return true;
    }
};

#endif