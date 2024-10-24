#ifndef _PIZZA_
#define _PIZZA_

#include <iostream>
#include <string>
using namespace std;

// Pizza class to store details of a pizza
class Pizza {
public:
    string name;
    float smallPrice;
    float mediumPrice;
    float largePrice;
    float extraLargePrice;

    Pizza() {}
    Pizza(string n, float s, float m, float l, float xl)
        : name(n), smallPrice(s), mediumPrice(m), largePrice(l), extraLargePrice(xl) {}
};

// Queue class to manage the orders in the kitchen
template <class T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;
    };
    
    Node* front;
    Node* rear;
    int numItems;

public:
    Queue() {
        front = nullptr;
        rear = nullptr;
        numItems = 0;
    }

    ~Queue() {
        clear();
    }

    bool isEmpty() {
        return numItems == 0;
    }

    void enqueue(T item) {
        Node* newNode = new Node;
        newNode->value = item;
        newNode->next = nullptr;

        if (isEmpty()) {
            front = newNode;
            rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        numItems++;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "The queue is empty." << endl;
            return;
        }

        Node* temp = front;
        front = front->next;
        delete temp;
        numItems--;
    }

    T getFront() {
        if (isEmpty()) {
            cout << "The queue is empty." << endl;
            return T();  // Return default value of T
        }
        return front->value;
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void printQueue() {
        Node* current = front;
        while (current != nullptr) {
            cout << current->value << endl;
            current = current->next;
        }
    }
};

#endif

