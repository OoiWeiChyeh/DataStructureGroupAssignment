#include "pizza.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;
// Function prototypes
void loadPizzas(Pizza pizzas[], int& count);
void displayMenu(const Pizza pizzas[], int count);
void takeOrder(Queue<Order>& orderQueue, const Pizza pizzas[], int count);
void sendToKitchen(Queue<Order>& orderQueue, Queue<Order>& kitchenQueue);
void processQueue(Queue<Order>& kitchenQueue);
void displayOrderSummary(Queue<Order>& orderQueue);
void displayKitchenQueue(Queue<Order>& kitchenQueue);

int main() {
    Pizza pizzas[10];
    int pizzaCount = 0;

    // Load pizzas from file
    loadPizzas(pizzas, pizzaCount);


    // Queue for storing orders
    Queue<Order> orderQueue;
    Queue<Order> kitchenQueue;
		
	cout << "Hi! Welcome to Pizza Kitchen  \n";
    int option = 0;
    while (option != 3) {
        cout << left << "\n1.  [Order Pizza" << right << setw(17) << " ]" ;
		cout << left << "\n2.  [Kitchen" << right << setw(21) << " ]";
		cout << left << "\n3.  [Exit" << right << setw(24) << " ]";
		cout << "\nSelect an option: ";
        cin >> option;

        switch (option) {
            case 1:
                takeOrder(orderQueue, pizzas, pizzaCount);
                displayOrderSummary(orderQueue);
                char confirm;
                cout << "Confirm send to the kitchen [Y/N]: ";
                cin >> confirm;
                if (tolower(confirm) == 'y') {
                    while (!orderQueue.isEmpty()) {
                        kitchenQueue.enqueue(orderQueue.getFront());
                        orderQueue.dequeue();
                        
                    }
                }
                break;
            case 2:
                displayKitchenQueue(kitchenQueue);
                break;
            case 3:
            	cout << "Exiting..." << endl;
            	break;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    }

    return 0;
}
