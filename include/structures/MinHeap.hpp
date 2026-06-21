#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <utility>
#include <limits>
#include "structures/DynamicArray.hpp"

// structure representing a single element inside the priority queue
struct HeapNode {
    int vertex;
    int key; // represents distance in Dijkstra or edge weight in Prim

    HeapNode() : vertex(-1), key(std::numeric_limits<int>::max()) {}
    HeapNode(int v, int k) : vertex(v), key(k) {}
};

class MinHeap {
private:
    DynamicArray<HeapNode> min_heap;

    // helper math inside array
    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t left(size_t i) const   { return 2 * i + 1; }
    size_t right(size_t i) const  { return 2 * i + 2; }

    // moves the element up to restore heap property after insertion
    void heapifyUp(size_t index) {
        while (index > 0 && min_heap[index].key < min_heap[parent(index)].key) {
            std::swap(min_heap[index], min_heap[parent(index)]);
            index = parent(index);
        }
    }

    // moves the element down to restore heap property after deletion
    void heapifyDown(size_t index) {
        size_t smallest = index;
        size_t l = left(index);
        size_t r = right(index);

        if (l < min_heap.size() && min_heap[l].key < min_heap[smallest].key) {
            smallest = l;
        }
        if (r < min_heap.size() && min_heap[r].key < min_heap[smallest].key) {
            smallest = r;
        }

        if (smallest != index) {
            std::swap(min_heap[index], min_heap[smallest]);
            heapifyDown(smallest); // recursive sink-down
        }
    }

public:
    MinHeap() = default;

    bool empty() const {
        return min_heap.size() == 0;
    }

    size_t size() const {
        return min_heap.size();
    }

    // inserts a new node into the priority queue
    void push(int vertex, int key) {
        min_heap.push_back(HeapNode(vertex, key));
        heapifyUp(min_heap.size() - 1);
    }

    // returns the root node (element with minimum key value)
    HeapNode top() const {
        if (empty()) {
            return HeapNode(-1, std::numeric_limits<int>::max());
        }
        return min_heap[0];
    }

    // removes the minimum element from the top of the heap
    void pop() {
        if (empty()) return;

        // move the last element to the root position and shrink the array size
        min_heap[0] = min_heap[min_heap.size() - 1];
        min_heap.pop_back();
    }
};

#endif