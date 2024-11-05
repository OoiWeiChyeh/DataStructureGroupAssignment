#ifndef PIZZA_H
#define PIZZA_H

#include <iostream>
#include <string>

using namespace std;

class Pizza {
public:
    string name;
    float smallPrice, mediumPrice, largePrice, extraLargePrice;

    // Constructor to initialize pizza details
    Pizza(const string& name = "", float small = 0, float medium = 0, float large = 0, float xl = 0)
        : name(name), smallPrice(small), mediumPrice(medium), largePrice(large), extraLargePrice(xl) {}
};

// Queue implementation for order and kitchen queue management
template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
    };
    Node* front;
    Node* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    bool isEmpty() const { return front == nullptr; }

    void enqueue(const T& item) {
        Node* newNode = new Node{item, nullptr};
        if (rear) {
            rear->next = newNode;
        } else {
            front = newNode;
        }
        rear = newNode;
    }

    void dequeue() {
        if (!isEmpty()) {
            Node* temp = front;
            front = front->next;
            delete temp;
            if (!front) rear = nullptr;
        }
    }

    T getFront() const {
        if (isEmpty()) throw "Queue is empty!";
        return front->data;
    }

    ~Queue() {
        while (!isEmpty()) dequeue();
    }
};

#endif

