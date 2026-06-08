#ifndef GRAPH_EDGE_HPP
#define GRAPH_EDGE_HPP

// POD (Plain Old Data); container for edge information (target vertex and weight) used in SuccessorList representation
struct GraphEdge {
    int target; // destination vertex index 
    int weight; // weight of the edge

    GraphEdge(): target(0), weight(0) {}
    GraphEdge(int t, int w): target(t), weight(w) {}
};

#endif