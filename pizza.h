#ifndef PIZZA_H
#define PIZZA_H

#include <iostream>
#include <queue>
#include <string>
using namespace std;

// Class for Pizza details
class Pizza {
public:
    string name;
    string size;
    int quantity;

    Pizza(string n = "", string s = "", int q = 0);
};

// Class for Order details
class Order {
public:
    Pizza pizza;
    int orderId;

    Order(int id = 0, Pizza p = Pizza());
};

// Function declarations
void loadPizzas(Pizza pizzas[], int& count, const string& filename);
void displayMenu(const Pizza pizzas[], int count);
void takeOrder(queue<Order>& orderQueue, const Pizza pizzas[], int count);
void displayOrderSummary(const queue<Order>& orderQueue);
void sendToKitchen(queue<Order>& orderQueue, queue<Order>& kitchenQueue);
void displayKitchenQueue(const queue<Order>& kitchenQueue);
void kitchenQueueMenu(queue<Order>& kitchenQueue);

#endif // PIZZA_H

