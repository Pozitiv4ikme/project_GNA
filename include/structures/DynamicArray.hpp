#ifndef ARRAY_HPP // if not defiend previously - define, if yes - skip (go to endif)
#define ARRAY_HPP

#include <stdexcept> // standart exceptions
#include <utility>

template <typename T>
class DynamicArray
{
private:
    T *data;             // pointer to the begining in memory
    size_t capacity;     // how much memory is alocated
    size_t current_size; // how many elements are now stored

    // resize to the new capacity
    void resize(size_t new_capacity)
    {
        T *new_data = new T[new_capacity];
        for (size_t i = 0; i < current_size; ++i)
        {
            new_data[i] = std::move(data[i]);
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    DynamicArray() : data(nullptr), capacity(0), current_size(0) {}

    // constructor with initial size
    explicit DynamicArray(size_t initial_size)
        : data(new T[initial_size]()), capacity(initial_size), current_size(initial_size) {}

    ~DynamicArray()
    {
        delete[] data; // free memory when object is destroyed
    }

    // Copy constructor
    DynamicArray(const DynamicArray &other)
        : data(new T[other.capacity]), capacity(other.capacity), current_size(other.current_size)
    {
        for (size_t i = 0; i < current_size; ++i)
        {
            data[i] = other.data[i];
        }
    }

    // Copy assignment operator
    DynamicArray &operator=(const DynamicArray &other)
    {
        if (this != &other)
        {
            T *new_data = new T[other.capacity];
            for (size_t i = 0; i < other.current_size; ++i)
            {
                new_data[i] = other.data[i];
            }
            delete[] data;
            data = new_data;
            capacity = other.capacity;
            current_size = other.current_size;
        }
        return *this;
    }

    /*
    It has been mathematically proven that a ratio less than 2 (for example, 1.5 or the golden ratio ≈1.618)
    is more efficient from the perspective of the OS memory manager. With a ratio of 1.5, memory freed up after
    previous resizes can be reused for subsequent resizes. With a ratio of 2, the next block is always larger
    than the sum of all previous ones, and it is more difficult to reuse old memory.
    */
    void push_back(const T &value)
    {
        if (current_size >= capacity)
        {
            resize(capacity == 0 ? 4 : capacity * 1.5);
        }
        data[current_size++] = value;
    }

    // return reference to the element at the given index
    T &operator[](size_t index)
    {
        if (index >= current_size)
        {
            throw std::out_of_range("Index out of range"); // throw exception if index is out of bounds
        }

        return data[index]; // return reference to the element at the given index
    }

    const T &operator[](size_t index) const
    {
        if (index >= current_size)
        {
            throw std::out_of_range("Index out of range"); // throw exception if index is out of bounds
        }
        return data[index];
    }

    // Getters for size and capacity
    size_t size() const { 
        return current_size;
    }

    // Resets the logical size to 0
    void clear() { current_size = 0; }
};

#endif