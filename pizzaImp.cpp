#include "pizza.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm> // For remove_if
#include <iostream>
#include <string>

int currentOrderID = 1;

// Function to generate a new order ID
std::string generateOrderID() {
    return "P" + std::to_string(currentOrderID++);
}

// Helper function to trim whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to load pizzas from the file
void loadPizzas(Pizza pizzas[], int& count) {
    std::ifstream file("pizza.txt");
    if (!file.is_open()) {
        std::cout << "Error: Unable to open pizza.txt file.\n";
        count = 0;
        return;
    }

    std::string line;
    count = 0;
    while (std::getline(file, line)) {
        line = trim(line); // Remove extraneous whitespace

        // Skip empty lines
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string name;
        float small, medium, large, xl;

        std::getline(ss, name, ',');
        name = trim(name);

        if (!(ss >> small)) {
            std::cout << "Error: Invalid data format in pizza.txt for line:\n" << line << std::endl;
            continue;
        }
        ss.ignore(1, ',');
        if (!(ss >> medium)) {
            std::cout << "Error: Invalid data format in pizza.txt for line:\n" << line << std::endl;
            continue;
        }
        ss.ignore(1, ',');
        if (!(ss >> large)) {
            std::cout << "Error: Invalid data format in pizza.txt for line:\n" << line << std::endl;
            continue;
        }
        ss.ignore(1, ',');
        if (!(ss >> xl)) {
            std::cout << "Error: Invalid data format in pizza.txt for line:\n" << line << std::endl;
            continue;
        }

        pizzas[count++] = Pizza(name, small, medium, large, xl);
    }
    file.close();
}

// Display menu for user to select pizza
void displayMenu(const Pizza pizzas[], int count) {
    std::cout << "<<Pizza Menu>>\n";
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << " [" << pizzas[i].name << "]\n";
    }
}

// Handle order selection
void takeOrder(Queue<std::string>& orderQueue, const Pizza pizzas[], int count) {
    std::string orderID = generateOrderID();
    float total = 0.0;
    char continueOrder;

    std::cout << "\nOrder ID: " << orderID << "\n";
    std::cout << std::setw(15) << "Pizza" << std::setw(10) << "Size" << std::setw(10) << "Qty" << std::setw(10) << "Price\n";
    std::cout << "-----------------------------------------------\n";

    do {
        int pizzaChoice = 0, sizeChoice = 0, quantity = 0;

        displayMenu(pizzas, count);
        std::cout << "Select Pizza: ";
        std::cin >> pizzaChoice;

        if (pizzaChoice < 1 || pizzaChoice > count) {
            std::cout << "Invalid pizza selection.\n";
            return;
        }

        const Pizza& selectedPizza = pizzas[pizzaChoice - 1];
        std::cout << selectedPizza.name << " Price List\n";
        std::cout << "1 [Personal - RM " << selectedPizza.smallPrice << "]\n";
        std::cout << "2 [Regular  - RM " << selectedPizza.mediumPrice << "]\n";
        std::cout << "3 [Large    - RM " << selectedPizza.largePrice << "]\n";
        std::cout << "4 [XLarge   - RM " << selectedPizza.extraLargePrice << "]\n";
        std::cout << "Select Pizza Size: ";
        std::cin >> sizeChoice;

        if (sizeChoice < 1 || sizeChoice > 4) {
            std::cout << "Invalid size selection.\n";
            return;
        }

        std::cout << "Enter Quantity: ";
        std::cin >> quantity;
        if (quantity <= 0) {
            std::cout << "Invalid quantity.\n";
            return;
        }

        // Calculate price based on size and quantity
        float price = 0.0;
        std::string sizeName;
        switch (sizeChoice) {
            case 1: price = selectedPizza.smallPrice * quantity; sizeName = "Personal"; break;
            case 2: price = selectedPizza.mediumPrice * quantity; sizeName = "Regular"; break;
            case 3: price = selectedPizza.largePrice * quantity; sizeName = "Large"; break;
            case 4: price = selectedPizza.extraLargePrice * quantity; sizeName = "XLarge"; break;
        }

        std::cout << std::setw(15) << selectedPizza.name
                  << std::setw(10) << sizeName
                  << std::setw(10) << quantity
                  << std::setw(10) << "RM " << price << "\n";

        total += price;
        orderQueue.enqueue(selectedPizza.name + " x" + std::to_string(quantity) + " (" + sizeName + ")");

        // Prompt to continue ordering
        std::cout << "Continue next pizza [Y/N]: ";
        std::cin >> continueOrder;

    } while (tolower(continueOrder) == 'y');

    std::cout << "-----------------------------------------------\n";
    std::cout << std::setw(35) << "Total: " << "RM " << total << "\n\n";
}

// Send orders from order queue to kitchen queue
void sendToKitchen(Queue<std::string>& orderQueue, Queue<std::string>& kitchenQueue) {
    while (!orderQueue.isEmpty()) {
        kitchenQueue.enqueue(orderQueue.getFront());
        orderQueue.dequeue();
    }
}

// Process kitchen queue
void processQueue(Queue<std::string>& kitchenQueue) {
    if (kitchenQueue.isEmpty()) {
        std::cout << "No orders in the kitchen queue.\n";
    } else {
        std::cout << "Now preparing: " << kitchenQueue.getFront() << std::endl;
        kitchenQueue.dequeue();
    }
} 
