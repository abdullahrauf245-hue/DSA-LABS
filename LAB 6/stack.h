/*
 * CS-250 Data Structures and Algorithms - Lab 6
 * stack.h - Stack ADT implemented from scratch as a singly linked list.
 *
 * Shared by Task 1 (expression conversion) and Task 2 (validators).
 * No std::stack / std::vector / std::deque is used.
 *
 * Operations (all O(1) except contains, which is O(n)):
 *   push(x)      - put x on top
 *   pop()        - remove and return the top element
 *   peek()       - look at the top element without removing it
 *   isEmpty()    - true if the stack has no elements
 *   size()       - number of elements
 *   contains(p)  - true if some element satisfies predicate p (top -> bottom)
 *
 * pop() and peek() throw std::underflow_error on an empty stack.
 */

#ifndef STACK_H
#define STACK_H

#include <stdexcept>

template <typename T>
class Stack {
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {}
    };
    Node* top_;
    int size_;

public:
    Stack() : top_(nullptr), size_(0) {}

    ~Stack() {
        while (!isEmpty()) pop();
    }

    // Copying would make two stacks own the same nodes, so it is disabled.
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const T& x) {
        top_ = new Node(x, top_);
        ++size_;
    }

    T pop() {
        if (top_ == nullptr) throw std::underflow_error("pop from empty stack");
        Node* n = top_;
        T value = n->data;
        top_ = n->next;
        delete n;
        --size_;
        return value;
    }

    const T& peek() const {
        if (top_ == nullptr) throw std::underflow_error("peek at empty stack");
        return top_->data;
    }

    bool isEmpty() const { return top_ == nullptr; }
    int size() const { return size_; }

    // Search from the top downwards. `match` returns true for the element we
    // are looking for (used for error recovery in the markup validator).
    template <typename Pred>
    bool contains(Pred match) const {
        for (Node* cur = top_; cur != nullptr; cur = cur->next)
            if (match(cur->data)) return true;
        return false;
    }
};

#endif  // STACK_H