#include "pizza.h"
#include <fstream>
#include <iomanip>
#include <sstream>

// Pizza constructor
Pizza::Pizza(string n, string s, int q) : name(n), size(s), quantity(q) {}

// Order constructor
Order::Order(int id, Pizza p) : orderId(id), pizza(p) {}

// Load pizzas from the file
void loadPizzas(Pizza pizzas[], int& count, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    count = 0;
    while (getline(file, line) && count < 10) {
        stringstream ss(line);
        string name, size;
        int quantity;

        ss >> ws;
        getline(ss, name, ',');
        ss >> size >> quantity;

        pizzas[count++] = Pizza(name, size, quantity);
    }

    file.close();
}

// Display available pizzas
void displayMenu(const Pizza pizzas[], int count) {
    cout << "\nAvailable Pizzas:\n";
    cout << left << setw(5) << "No." << setw(20) << "Name" << setw(10) << "Size" << "Quantity\n";
    for (int i = 0; i < count; ++i) {
        cout << left << setw(5) << i + 1 << setw(20) << pizzas[i].name
             << setw(10) << pizzas[i].size << pizzas[i].quantity << "\n";
    }
}

// Take an order
void takeOrder(queue<Order>& orderQueue, const Pizza pizzas[], int count) {
    int choice, quantity;
    cout << "\nChoose a pizza by number: ";
    cin >> choice;

    // Input validation
    if (cin.fail() || choice < 1 || choice > count) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Invalid input. Please select a valid pizza number.\n";
        return;
    }

    cout << "Enter quantity: ";
    cin >> quantity;

    if (cin.fail() || quantity <= 0) {
        cin.clear();
        cin.ignore(INT_MAX, '\n');
        cout << "Invalid input. Please enter a positive quantity.\n";
        return;
    }

    Pizza selectedPizza = pizzas[choice - 1];
    selectedPizza.quantity = quantity;

    static int orderId = 1;
    orderQueue.push(Order(orderId++, selectedPizza));
}

// Display order summary
void displayOrderSummary(const queue<Order>& orderQueue) {
    queue<Order> tempQueue = orderQueue;
    cout << "\nCurrent Orders:\n";

    while (!tempQueue.empty()) {
        Order order = tempQueue.front();
        tempQueue.pop();

        cout << "Order ID: " << order.orderId << ", Pizza: " << order.pizza.name
             << " - " << order.pizza.size << " x" << order.pizza.quantity << "\n";
    }
}

// Send orders to kitchen queue
void sendToKitchen(queue<Order>& orderQueue, queue<Order>& kitchenQueue) {
    while (!orderQueue.empty()) {
        kitchenQueue.push(orderQueue.front());
        orderQueue.pop();
    }
}

// Display the kitchen queue
void displayKitchenQueue(const queue<Order>& kitchenQueue) {
    if (kitchenQueue.empty()) {
        cout << "\nNo pizzas in the kitchen queue.\n";
        return;
    }

    queue<Order> tempQueue = kitchenQueue;
    int position = 1;

    cout << "\nKitchen Queue:\n";
    while (!tempQueue.empty()) {
        Order order = tempQueue.front();
        tempQueue.pop();

        cout << "+----------------------------+\n";
        cout << "| [" << position << "] " << order.pizza.name << "-" << order.pizza.size
             << " x" << order.pizza.quantity;
        if (position == 1) cout << " [Preparing]";
        cout << " |\n+----------------------------+\n";

        ++position;
    }
}

// Kitchen queue menu
void kitchenQueueMenu(queue<Order>& kitchenQueue) {
    int option;
    do {
        displayKitchenQueue(kitchenQueue);

        cout << "\n[1] Prepare Next Pizza\n[2] Cancel Pizza\n[3] Insert Pizza\n[4] Reset Queue\n[5] Quit\n";
        cout << "Select an option: ";
        cin >> option;

        switch (option) {
            case 1: // Prepare next pizza
                if (!kitchenQueue.empty()) {
                    kitchenQueue.pop();
                    cout << "Next pizza is now being prepared.\n";
                } else {
                    cout << "No pizzas to prepare.\n";
                }
                break;

            case 2: // Cancel pizza
                // Code here
                break;

            case 3: // Insert pizza
                // Code here
                break;

            case 4: // Reset queue
                while (!kitchenQueue.empty()) kitchenQueue.pop();
                cout << "All pizzas cleared.\n";
                break;

            case 5:
                cout << "Exiting kitchen menu...\n";
                break;

            default:
                cout << "Invalid input.\n";
        }
    } while (option != 5);
}

