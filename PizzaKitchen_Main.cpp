#include "pizza.h"
#include "Order.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// Function prototypes for the pizza ordering system
void loadPizzas(Pizza pizzas[], int& count);              // Loads pizza data from file
string trim(const string& str);                           // Removes leading/trailing whitespace
void displayMenu(const Pizza pizzas[], int count);        // Shows available pizzas and prices
void takeOrder(OrderQueue& orderQueue, const Pizza pizzas[], int count);  // Handles customer order input
void sendToKitchen(OrderQueue& orderQueue, OrderQueue& kitchenQueue);     // Transfers orders to kitchen
void processQueue(OrderQueue& kitchenQueue);              // Processes orders in kitchen
void displayOrderSummary(OrderQueue& orderQueue);         // Shows order details and total
void displayKitchenQueue(OrderQueue& kitchenQueue);       // Displays current kitchen orders
void kitchenQueueMenu(OrderQueue& kitchenQueue, const Pizza pizzas[], int pizzaCount);  // Kitchen management interface
void cancelPizza(OrderQueue& orderQueue);                 // Removes a pizza from the order
void insertPizza(OrderQueue& orderQueue, const Pizza pizzas[], int pizzaCount);  // Adds a pizza to specific position

int main() {
    // Initialize system variables
    Pizza pizzas[10];                // Array to store pizza menu items
    int pizzaCount = 0;              // Number of pizzas loaded from file
    OrderQueue orderQueue;           // Queue for current customer orders
    OrderQueue kitchenQueue;         // Queue for orders being prepared

    // Load pizza menu data from file
    loadPizzas(pizzas, pizzaCount);

    // Main program loop
    int option = 0;
    while (option != 3) {
        // Display main menu
        cout << "Hi! Welcome to Pizza Kitchen\n";
        cout << "----------------------------\n";
        cout << left << "1. [Order Pizza]\t   |\n";
        cout << left << "2. [Kitchen]\t\t   |\n";
        cout << left << "3. [Exit]\t\t   |\n";
        cout << "----------------------------\n";
        cout << "Select an option: ";
        cin >> option;

        // Handle menu selection
        switch (option) {
            case 1: // Order pizza
                takeOrder(orderQueue, pizzas, pizzaCount);
                displayOrderSummary(orderQueue);

                // Confirm order submission
                char confirm;
                cout << "Confirm send to the kitchen [Y/N]: ";
                cin >> confirm;

                if (toupper(confirm) != 'Y') {
                    cout << "Order send to the kitchen unsuccessful.\n\n";
                } else {
                    sendToKitchen(orderQueue, kitchenQueue);
                    kitchenQueueMenu(kitchenQueue, pizzas, pizzaCount);
                }
                break;

            case 2: // Kitchen management
                if (kitchenQueue.isEmpty()) {
                    cout << "No orders currently in the kitchen queue.\n";
                } else {
                    kitchenQueueMenu(kitchenQueue, pizzas, pizzaCount);
                }
                break;

            case 3: // Exit program
                cout << "Exiting... Thank you for visiting Pizza Kitchen!\n";
                break;

            default:
                cout << "Invalid option. Please try again.\n";
        }
    }

    return 0;
}

// Global order ID counter
int currentOrderID = 1000;

// Generates unique order ID with 'P' prefix
string generateOrderID() {
    return "P" + to_string(currentOrderID++);
}

// Handles the pizza ordering process
void takeOrder(OrderQueue& orderQueue, const Pizza pizzas[], int count) {
    string orderID = generateOrderID();
    char continueOrder;

    cout << "\n\nOrder ID: " << orderID << "\n";

    do {
        int pizzaChoice = 0, sizeChoice = 0, quantity = 0;

        // Display menu and get pizza selection with validation
        displayMenu(pizzas, count);
        bool validPizzaChoice = false;
        
        while (!validPizzaChoice) {
            cout << "\nSelect Pizza (1-" << count << "): ";
            
            // Check if input is a number
            if (!(cin >> pizzaChoice)) {
                cin.clear(); // Clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            
            // Validate pizza choice range
            if (pizzaChoice < 1 || pizzaChoice > count) {
                cout << "Invalid pizza selection. Please choose between 1 and " << count << ".\n";
                continue;
            }
            
            validPizzaChoice = true;
        }
        cout << endl;

        // Display size options and prices for selected pizza
        const Pizza& selectedPizza = pizzas[pizzaChoice - 1];
        cout << selectedPizza.name << " Price List\n\n";
        cout << "1 [Personal - RM  " << selectedPizza.smallPrice << "]\n";
        cout << "2 [Regular  - RM " << selectedPizza.mediumPrice << "]\n";
        cout << "3 [Large    - RM " << selectedPizza.largePrice << "]\n";
        cout << "4 [XLarge   - RM " << selectedPizza.extraLargePrice << "]\n\n";
        
        // Size selection with validation
        bool validSizeChoice = false;
        while (!validSizeChoice) {
            cout << "Select Pizza Size (1-4): ";
            
            // Check if input is a number
            if (!(cin >> sizeChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            
            if (sizeChoice < 1 || sizeChoice > 4) {
                cout << "Invalid size selection. Please choose between 1 and 4.\n";
                continue;
            }
            
            validSizeChoice = true;
        }

        // Quantity validation
        bool validQuantity = false;
        while (!validQuantity) {
            cout << "Enter Quantity: ";
            
            // Check if input is a number
            if (!(cin >> quantity)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }
            
            // Validate quantity range 
            if (quantity < 1) {
                cout << "Invalid quantity. Please enter between 1 and 20.\n";
                continue;
            }
            
            validQuantity = true;
        }

        // Calculate price based on size and quantity
        float price = 0.0;
        string sizeName;
        switch (sizeChoice) {
            case 1: price = selectedPizza.smallPrice * quantity; sizeName = "Personal"; break;
            case 2: price = selectedPizza.mediumPrice * quantity; sizeName = "Regular"; break;
            case 3: price = selectedPizza.largePrice * quantity; sizeName = "Large"; break;
            case 4: price = selectedPizza.extraLargePrice * quantity; sizeName = "XLarge"; break;
        }

        // Create and add order to queue
        Order newOrder(orderID, selectedPizza.name, sizeChoice, quantity, price);
        orderQueue.enqueue(newOrder);

        cout << "Added to order: " << selectedPizza.name << " x" << quantity << endl;

        // Validate continue order input
        bool validContinueChoice = false;
        while (!validContinueChoice) {
            cout << "\nContinue next pizza [Y/N]: ";
            cin >> continueOrder;
            continueOrder = toupper(continueOrder);
            
            if (continueOrder != 'Y' && continueOrder != 'N') {
                cout << "Invalid input. Please enter Y or N.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            validContinueChoice = true;
        }
        cout << endl;
        
    } while (toupper(continueOrder) == 'Y');
}

// Displays order summary with tax calculation
void displayOrderSummary(OrderQueue& orderQueue) {
    float total = 0.0;
    int i = 1;

    // Print header
    cout << "{Order Summary}:\n\n";
    cout << left << "Pizza " << right << setw(24) << " Quantity " << right << setw(15) << " Total\n";
    cout << setfill('-') << setw(44) << "-" << setfill(' ') << endl << endl; 

    // Create temporary queue to preserve order
    OrderQueue tempQueue;
    while (!orderQueue.isEmpty()) {
        Order order = orderQueue.dequeue();
        float itemTotal = order.priceItem;
        total += itemTotal;

        // Display each item
        cout << left << i++ << "." << setw(18) << order.pizzaName
             << right << setw(8) << order.quantity << right << setw(16) << fixed << setprecision(2) << itemTotal << endl;

        tempQueue.enqueue(order);
    }

    // Restore original queue
    while (!tempQueue.isEmpty()) {
        orderQueue.enqueue(tempQueue.dequeue());
    }

    // Calculate and display tax and final total
    double tax = total * 0.05;
    double lastTotal = total + tax;
    cout << "--------------------------------------------\n";
    cout << right << setw(40) <<"TAX(5%): "<< tax << endl;
    cout << right << setw(39) << "TOTAL(RM):" << lastTotal << endl;
}

// Transfers orders from order queue to kitchen queue
void sendToKitchen(OrderQueue& orderQueue, OrderQueue& kitchenQueue) {
    if (orderQueue.isEmpty()) {
        cout << "Don't have any order send to the kitchen.\n";
        return;
    }
    while (!orderQueue.isEmpty()) {
        kitchenQueue.enqueue(orderQueue.dequeue());
    }
    cout << "All orders have been successfully sent to the kitchen.\n";
}

void displayKitchenQueue(OrderQueue& kitchenQueue) {
    if (kitchenQueue.isEmpty()) {
        cout << "No orders in the kitchen queue.\n";
        return;
    }
    
    OrderQueue tempQueue;
    int orderNum = 1;

    while (!kitchenQueue.isEmpty()) {
        Order order = kitchenQueue.dequeue();
        string size;
        switch (order.size) {
            case 1: size = "S"; break;
            case 2: size = "M"; break;
            case 3: size = "L"; break;
            case 4: size = "XL"; break;
        }

        if (orderNum == 1) {
            cout << "+-------------------------------------+\n";
            cout << "| [" << orderNum << "] " << left << setw(20) << (order.pizzaName + "-" + size + " x " + to_string(order.quantity))
                 << "[Preparing...]|\n";
            cout << "+-------------------------------------+\n";
        } else {
            cout << "+---------------------------+\n";
            cout << "| [" << orderNum << "] " << left << setw(20) << (order.pizzaName + "-" + size + " x " + to_string(order.quantity))
                 << "|\n";
            cout << "+---------------------------+\n";
        }

        tempQueue.enqueue(order);
        orderNum++;
        if (!kitchenQueue.isEmpty()) cout << "          |\n          |\n          V\n";
    }

    while (!tempQueue.isEmpty()) {
        kitchenQueue.enqueue(tempQueue.dequeue());
    }
}

// Kitchen management interface
void kitchenQueueMenu(OrderQueue& kitchenQueue, const Pizza pizzas[], int pizzaCount) {
    int choice;
    do {
        displayKitchenQueue(kitchenQueue);
        // Display kitchen management options
        cout << "\nKitchen Queue Management:\n";
        cout << "1. Prepare Next Pizza\n";
        cout << "2. Cancel Pizza\n";
        cout << "3. Insert Pizza\n";
        cout << "4. Reset Queue\n";
        cout << "5. Quit\n";
        cout << "Choose an option: ";
        cin >> choice;

        // Handle kitchen management options
        switch (choice) {
            case 1: // Process next order
                if (!kitchenQueue.isEmpty()) {
                    kitchenQueue.dequeue();
                    cout << "Next pizza is being prepared.\n";
                } else {
                    cout << "No pizzas to prepare.\n";
                }
                break;
            case 2: // Remove specific order
                cancelPizza(kitchenQueue);
                break;
            case 3: // Add new order at specific position
                insertPizza(kitchenQueue, pizzas, pizzaCount);
                break;
            case 4: // Clear all orders
                while (!kitchenQueue.isEmpty()) kitchenQueue.dequeue();
                cout << "Kitchen queue has been reset.\n";
                break;
            case 5:
                cout << "Exiting kitchen queue management.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
}

// Loads pizza menu data from file
void loadPizzas(Pizza pizzas[], int& count) {
    ifstream file("pizza.txt");

    if (!file) {
        cout << "Error: pizza.txt file not found...\n";
        return;
    }

    string line;
    count = 0;

    // Read and parse each line from file
    while (getline(file, line)) {
        line = trim(line); // Remove whitespace

        if (line.empty()) continue;

        istringstream ss(line);
        string name;
        float small, medium, large, xl;

        // Parse pizza data
        getline(ss, name, ',');
        name = trim(name);

        // Validate and read prices
        if (!(ss >> small)) {
            cout << "Error: Invalid data format in pizza.txt for line:\n" << line << endl;
            continue;
        }
        ss.ignore(1, ',');
        if (!(ss >> medium)) {
            cout << "Error: Invalid data format in pizza.txt for line:\n" << line << endl;
            continue;
        }
        ss.ignore(1, ',');
        if (!(ss >> large)) {
            cout << "Error: Invalid data format in pizza.txt for line:\n" << line << endl;
            continue;
        }
        ss.ignore(1, ',');
        if (!(ss >> xl)) {
            cout << "Error: Invalid data format in pizza.txt for line:\n" << line << endl;
            continue;
        }

        // Create new pizza entry
        pizzas[count] = Pizza(name, small, medium, large, xl);
        count++;
    }
    file.close();

    if (count == 0) {
        cout << "Error: No valid pizzas found in pizza.txt...\n";
    }
}

// Displays pizza menu with prices
void displayMenu(const Pizza pizzas[], int count) {
    cout << "\n=================================== Pizza Menu =================================\n\n";
    cout << left << setw(25) << "[Pizza Name]" << setw(15) << "Small ($)" << setw(15)
         << "Medium ($)" << setw(15) << "Large ($)" << setw(15) << "XL ($)" << endl;
    cout << "--------------------------------------------------------------------------------\n\n";
    
    // Display each pizza with prices
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << left << setw(18) << pizzas[i].name
             << right << fixed << setw(12) << setprecision(2) << pizzas[i].smallPrice
             << setw(16) << pizzas[i].mediumPrice
             << setw(12) << pizzas[i].largePrice
             << setw(15) << pizzas[i].extraLargePrice << endl;
    }
}

// Removes leading/trailing whitespace from string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Removes a pizza from specified position in queue
void cancelPizza(OrderQueue& orderQueue) {
    int position;

    if (orderQueue.isEmpty() || orderQueue.size() == 1) {
        cout << "No orders available to cancel, or only the first item exists.\n";
        return;
    }

    cout << "Enter the position to cancel (not the first item): ";
    cin >> position;

    if (orderQueue.deleteAtPosition(position)) {
        cout << "Order at position [" << position << "] canceled successfully.\n";
    } else {
        cout << "Failed to cancel order.\n";
    }
}

// Inserts a new pizza order at specified position
void insertPizza(OrderQueue& orderQueue, const Pizza pizzas[], int pizzaCount) {
    int position;

    cout << "Enter the position to insert the order (not the first item): ";
    cin >> position;

    // Validate insertion position
    if (position <= 1 || position > orderQueue.size() + 1) {
        cout << "Invalid position. Cannot insert.\n";
        return;
    }

    OrderQueue tempQueue;

    // Move orders before insertion point to temp queue

    // Extract orders up to the insertion point
    for (int i = 1; i < position && !orderQueue.isEmpty(); ++i) {
        tempQueue.enqueue(orderQueue.dequeue());
    }

    // Use the `takeOrder` function to create a new order directly
    cout << "Creating new order for insertion...\n";
    takeOrder(tempQueue, pizzas, pizzaCount); // Inserts directly into the tempQueue

    // Reinsert remaining orders from the original queue
    while (!orderQueue.isEmpty()) {
        tempQueue.enqueue(orderQueue.dequeue());
    }

    // Restore all orders, including the new one, back to the original queue
    while (!tempQueue.isEmpty()) {
        orderQueue.enqueue(tempQueue.dequeue());
    }

    cout << "Order successfully inserted at position [" << position << "].\n";
}


