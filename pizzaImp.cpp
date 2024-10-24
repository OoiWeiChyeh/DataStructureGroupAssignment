#include "pizza.h"
#include <fstream>
#include <sstream>

// Function to load pizzas from file (pizza.txt)
void loadPizzas(Pizza pizzas[], int& count) {
    ifstream file("pizza.txt");
    string line;
    count = 0;

    while (getline(file, line)) {
        istringstream ss(line);
        string name;
        float small, medium, large, xl;
        getline(ss, name, ',');
        ss >> small >> medium >> large >> xl;
        pizzas[count] = Pizza(name, small, medium, large, xl);
        count++;
    }
    file.close();
}

// Function to display available pizzas
void displayMenu(const Pizza pizzas[], int count) {
    cout << "Available Pizzas:\n";
    for (int i = 0; i < count; i++) {
        cout << i+1 << ". " << pizzas[i].name << " (Small: $" << pizzas[i].smallPrice << ", Medium: $" 
             << pizzas[i].mediumPrice << ", Large: $" << pizzas[i].largePrice << ", Extra Large: $" 
             << pizzas[i].extraLargePrice << ")" << endl;
    }
}

// Function to handle ordering
void takeOrder(Queue<string>& orderQueue, const Pizza pizzas[], int count) {
    int choice = 0;
    while (choice != -1) {
        displayMenu(pizzas, count);
        cout << "Enter the pizza number to order (-1 to finish): ";
        cin >> choice;

        if (choice > 0 && choice <= count) {
            orderQueue.enqueue(pizzas[choice - 1].name);
            cout << pizzas[choice - 1].name << " added to your order." << endl;
        } else if (choice != -1) {
            cout << "Invalid selection. Please try again." << endl;
        }
    }
}

// Function to send orders to kitchen queue
void sendToKitchen(Queue<string>& orderQueue, Queue<string>& kitchenQueue) {
    while (!orderQueue.isEmpty()) {
        kitchenQueue.enqueue(orderQueue.getFront());
        orderQueue.dequeue();
    }
}

// Function to process orders in the kitchen queue
void processQueue(Queue<string>& kitchenQueue) {
    if (kitchenQueue.isEmpty()) {
        cout << "No orders in the kitchen queue." << endl;
    } else {
        string currentPizza = kitchenQueue.getFront();
        cout << "Now preparing: " << currentPizza << endl;
        kitchenQueue.dequeue();
    }
}

