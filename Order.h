#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <iostream>
#include "pizza.h"

class Order {
public:
    std::string orderID;
    std::string pizzaName;
    int size;   // 1: Small, 2: Medium, 3: Large, 4: XLarge
    int quantity;
    float priceItem;

    // Default constructor
    Order() : size(0), quantity(0), priceItem(0.0) {}

    // Parameterized constructor
    Order(const std::string& id, const std::string& name, int pizzaSize, 
           int qty, float price) 
        : orderID(id), pizzaName(name), size(pizzaSize), 
          quantity(qty), priceItem(price) {}

    // Display method for debugging
    void display() const {
        std::cout << "Order ID: " << orderID << std::endl;
        std::cout << "Pizza: " << pizzaName << std::endl;
        std::cout << "Size: " << size << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
        std::cout << "Price: " << priceItem << std::endl;
    }
};

// Queue implementation follows...
class OrderQueue {
private:
    struct Node {
        Order data;
        Node* next;
    };
    Node* front;
    Node* rear;
    int numItems;

public:
    // Constructor
    OrderQueue() : front(nullptr), rear(nullptr), numItems(0) {}

    // Destructor
    ~OrderQueue() {
        clear();
    }

    // Check if the queue is empty
    bool isEmpty() const {
        return numItems == 0;
    }

    // Enqueue an Order
    void enqueue(const Order& item) {
        Node* newNode = new Node{item, nullptr};
        if (rear != nullptr) {
            rear->next = newNode;
        } else {
            front = newNode;
        }
        rear = newNode;
        numItems++;
    }

    // Dequeue an Order - returns the Order
    Order dequeue() {
        if (!isEmpty()) {
	        Node* temp = front;
	        Order item = front->data;
	        front = front->next;
	        if (front == nullptr) rear = nullptr;
	        delete temp;
	        numItems--;
	        return item;
		}
    }

    // Get the front Order without removing
    Order getFront() const {
        if (!isEmpty()){
	        return front->data;
		}
    }

    // Clear the queue
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Get the number of items in the queue
    int size() const {
        return numItems;
    }
    
    // Delete a pizza at a specific position (excluding the first)
	bool deleteAtPosition(int position) {
	    if (position <= 1 || position > numItems) {
	        cout << "Invalid position. Cannot delete.\n";
	        return false;
	    }
	
	    Node* current = front;
	    Node* previous = nullptr;
	
	    for (int i = 1; i < position; ++i) {
	        previous = current;
	        current = current->next;
	    }
	
	    previous->next = current->next;
	    if (current == rear) rear = previous;
	    delete current;
	    numItems--;
	
	    return true;
	}
	
	// Insert an order at a specific position (excluding the first)
	bool insertAtPosition(int position, const Order& item) {
	    if (position <= 1 || position > numItems + 1) {
	        cout << "Invalid position. Cannot insert.\n";
	        return false;
	    }
	
	    Node* newNode = new Node{item, nullptr};
	
	    if (position == numItems + 1) { // Insert at the end
	        if (rear) rear->next = newNode;
	        rear = newNode;
	    } else { // Insert in the middle
	        Node* current = front;
	        Node* previous = nullptr;
	
	        for (int i = 1; i < position; ++i) {
	            previous = current;
	            current = current->next;
	        }
	
	        newNode->next = current;
	        if (previous) previous->next = newNode;
	    }
	
	    numItems++;
	    return true;
	}
};

#endif // ORDER_H
