#include "pizza.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm> // For remove_if
#include <iostream>
#include <string>

int currentOrderID = 1;

using namespace std;

// Function to generate a new order ID
string generateOrderID() {
    return "P" + to_string(currentOrderID++);
}

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

// Function to load pizzas from file (pizza.txt)
void loadPizzas(Pizza pizzas[], int& count) {
    ifstream file("pizza.txt");

    if (!file) {
        cout << "Error: pizza.txt file not found...\n";
        return;
    }

    string line;
    count = 0;

    while (getline(file, line)) {
        line = trim(line); // Remove extraneous whitespace

        // Skip empty lines
        if (line.empty()) continue;

        istringstream ss(line);
        string name;
        float small, medium, large, xl;

        getline(ss, name, ',');
        name = trim(name);

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

        pizzas[count] = Pizza(name, small, medium, large, xl);
        count++; // Increment count for each valid pizza entry
    }
    file.close();

    if (count == 0) {
        cout << "Error: No valid pizzas found in pizza.txt...\n";
    }
}

void kitchenQueueMenu(Queue<Order>& kitchenQueue) {
    int choice;
    do {
        cout << "\nKitchen Queue Management:\n";
        cout << "1. Prepare Next Pizza\n";
        cout << "2. Cancel Pizza\n";
        cout << "3. Insert Pizza\n";
        cout << "4. Reset Queue\n";
        cout << "5. Quit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (!kitchenQueue.isEmpty()) {
                    cout << "Preparing: " << kitchenQueue.getFront().pizzaName << endl;
                    kitchenQueue.dequeue();
                } else {
                    cout << "No pizzas in the queue." << endl;
                }
                break;
            case 2: {
                if (!kitchenQueue.isEmpty()) {
                    cout << "Cancelling: " << kitchenQueue.getFront().pizzaName << endl;
                    kitchenQueue.dequeue();
                } else {
                    cout << "No pizzas to cancel." << endl;
                }
                break;
            }
            case 3: {
                string pizzaName;
                int size, quantity;
                cout << "Enter pizza name, size (1-4), and quantity: ";
                cin >> pizzaName >> size >> quantity;
                Order newOrder("Custom", pizzaName, size, quantity, 0);
                kitchenQueue.enqueue(newOrder);
                cout << "Pizza inserted into queue." << endl;
                break;
            }
            case 4:
                kitchenQueue.resetQueue();
                cout << "Queue has been reset." << endl;
                break;
            case 5:
                cout << "Exiting kitchen management.\n";
                break;
            default:
                cout << "Invalid option.\n";
        }
    } while (choice != 5);
}

//function to display order summary
void displayOrderSummary(Queue<Order>& orderQueue){
	float total = 0.0;
	int i = 1;
	
	cout << "{Order Summary}:\n\n";
	cout << left << "Pizza " << right << setw(15) << " Quantity " << right << setw(10) << " Total\n";
	cout << setfill('-') << setw(28) << "-" << setfill(' ') << endl << endl; 
	
	
	Queue<Order> tempQueue = orderQueue;
	while (!tempQueue.isEmpty()){
		Order order = tempQueue.getFront();
		float itemTotal = order.priceItem;
		total += itemTotal;
		
		cout << left << i++ << "." << setw(18) << order.pizzaName
			 << right << setw(10) << order.quantity << fixed << setprecision(2) << itemTotal << endl;
			 
		tempQueue.dequeue();
	}
	double tax = total * 0.05;
    
    cout << "------------------------------------------------------\n";
    cout << "TAX(5%) " << right << setw(15) << " :" << tax << endl;
    cout << "TOTAL (RM)" << right << setw(12) << cout <<" :"  << (total + tax) << endl;
}

// Function to display kitchen queue
void displayKitchenQueue(Queue<Order>& kitchenQueue) {
    Queue<Order> tempQueue = kitchenQueue;
    int orderNum = 1;

    while (!tempQueue.isEmpty()) {
        Order order = tempQueue.getFront();
        string size;
        switch (order.size) {
            case 1: size = "S"; break;
            case 2: size = "M"; break;
            case 3: size = "L"; break;
            case 4: size = "XL"; break;
        }

        cout << "+----------------------------+\n";
        cout << "| [" << orderNum++ << "] " << order.pizzaName << "-" << size
             << " x " << order.quantity << "       | [Preparing...]\n";
        cout << "+----------------------------+\n";
        
        tempQueue.dequeue();
        if (!tempQueue.isEmpty()) cout << "          |\n          |\n          V\n";
    }
}

// Function to display available pizzas
void displayMenu(const Pizza pizzas[], int count) {
    cout << "\n=================================== Pizza Menu =================================\n\n";
    cout << left << setw(25) << "[Pizza Name]" << setw(15) << "Small ($)" << setw(15)
         << "Medium ($)" << setw(15) << "Large ($)" << setw(15) << "XL ($)" << endl;
    cout << "--------------------------------------------------------------------------------\n\n";
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << left << setw(18) << pizzas[i].name
             << right << fixed << setw(12) << setprecision(2) << pizzas[i].smallPrice
             << setw(16) << pizzas[i].mediumPrice
             << setw(12) << pizzas[i].largePrice
             << setw(15) << pizzas[i].extraLargePrice << endl;
    }
}




// Function to handle ordering
void takeOrder(Queue<Order>& orderQueue, const Pizza pizzas[], int count) {
    string orderID = generateOrderID();
    float total = 0.0;
    char continueOrder;

    cout << "\n\nOrder ID: " << orderID << "\n";

    do {
        int pizzaChoice = 0, sizeChoice = 0, quantity = 0;

        displayMenu(pizzas, count);
        cout << "\nSelect Pizza: ";
        cin >> pizzaChoice;
		cout << endl;
        if (pizzaChoice < 1 || pizzaChoice > count) {
            cout << "Invalid pizza selection.\n";
            return;
        }

        const Pizza& selectedPizza = pizzas[pizzaChoice - 1];
        cout << selectedPizza.name << " Price List\n\n";
        cout << "1 [Personal - RM " << selectedPizza.smallPrice << "]\n";
        cout << "2 [Regular  - RM " << selectedPizza.mediumPrice << "]\n";
        cout << "3 [Large    - RM " << selectedPizza.largePrice << "]\n";
        cout << "4 [XLarge   - RM " << selectedPizza.extraLargePrice << "]\n\n";
        cout << "Select Pizza Size: ";
        cin >> sizeChoice;

        while (sizeChoice < 1 || sizeChoice > 4){
        	cout << "Please select the size that we have....\n";
        	cout << "---------------------------------------\n";
        	cout << "Select Pizza Size: ";
        	cin >> sizeChoice;
		}

        cout << "Enter Quantity: ";
        cin >> quantity;

        // Calculate price based on size and quantity
        float price = 0.0;
        string sizeName;
        switch (sizeChoice) {
            case 1: price = selectedPizza.smallPrice * quantity; sizeName = "Personal"; break;
            case 2: price = selectedPizza.mediumPrice * quantity; sizeName = "Regular"; break;
            case 3: price = selectedPizza.largePrice * quantity; sizeName = "Large"; break;
            case 4: price = selectedPizza.extraLargePrice * quantity; sizeName = "XLarge"; break;
        }

        

        Order newOrder(orderID, selectedPizza.name, sizeChoice, quantity, price);
        orderQueue.enqueue(newOrder);

        cout << "Added to order: " << selectedPizza.name << " x" << quantity << endl;

        // Prompt to continue ordering
        cout << "\nContinue next pizza [Y/N]: ";
        cin >> continueOrder;
		cout << endl;
    } while (tolower(continueOrder) == 'y');

    
}

// Function to send orders to kitchen queue
void sendToKitchen(Queue<Order>& orderQueue, Queue<Order>& kitchenQueue) {
    while (!orderQueue.isEmpty()) {
        kitchenQueue.enqueue(orderQueue.getFront());
        orderQueue.dequeue();
    }
}

// Function to process orders in the kitchen queue
void processQueue(Queue<Order>& kitchenQueue) {
    if (kitchenQueue.isEmpty()) {
        cout << "No orders in the kitchen queue." << endl;
    } else {
        Order currentOrder = kitchenQueue.getFront();

        cout << "Now preparing: " << kitchenQueue.getFront().pizzaName
             << " x" << currentOrder.quantity << " (" << currentOrder.size << ")" << endl;
        kitchenQueue.dequeue();
    }
}
