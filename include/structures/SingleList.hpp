#ifndef SINGLELIST_HPP
#define SINGLELIST_HPP

template <typename T>
class SingleList
{
private:
    Node *head; // pointer to the first element in the list
    Node *tail; // pointer to the last element in the list
    int current_size;

public:
    // algorithms can iterate directly via node pointers in O(1)
    struct Node
    {
        T data;
        Node *next;

        Node(const T &value) : data(value), next(nullptr) {}
    };

    SingleList() : head(nullptr), tail(nullptr), current_size(0) {}

    // Copy constructor
    SingleList(const SingleList& other) : head(nullptr), tail(nullptr), current_size(0) {
        Node* current = other.head;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Assignment operator
    SingleList& operator=(const SingleList& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    ~SingleList()
    {
        Node *current = head; // start from the head of the list
        while (current != nullptr)
        {
            Node *next = current->next; // save pointer to the next element

            // // for checking work of destructor after main() finishes
            // cout << "Destructor: burning node with data " << current->data << "!" << endl;

            delete current;             // free memory of the current element
            current = next;             // move to the next element
        }

        head = nullptr;
        tail = nullptr;
        current_size = 0;
    }

    int size() const
    {
        return current_size;
    }

    Node* getHead() const 
    {
        return head;
    }

    void push_back(const T &value)
    {
        Node *new_node = new Node(value);

        // if the list is empty, new node becomes both head and tail
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node; // link new node to the previous last element
            tail = new_node; // update tail to the new last element
        }
        current_size++;
    }

    T &operator[](int index)
    {
        if (index < 0 || index >= current_size)
        {
            throw std::out_of_range("Index out of range"); // throw exception if index is out of bounds
        }

        Node* current = head; // start from the head of the list
        for (int i = 0; i < index; ++i)
        {
            current = current->next; // move to the next element until we reach the needed index
        }

        return current->data; // return reference to the data of element
    }
};

#endif