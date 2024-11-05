#include "pizza.h"
#include <iostream>
#include <iomanip>

// Function prototypes
void loadPizzas(Pizza pizzas[], int& count);
void displayMenu(const Pizza pizzas[], int count);
void takeOrder(Queue<string>& orderQueue, const Pizza pizzas[], int count);
void sendToKitchen(Queue<string>& orderQueue, Queue<string>& kitchenQueue);
void processQueue(Queue<string>& kitchenQueue);

int main() {
    Pizza pizzas[10];
    int pizzaCount;

    // Load pizzas from file
    loadPizzas(pizzas, pizzaCount);

    // Queue for orders and kitchen processing
    Queue<string> orderQueue;
    Queue<string> kitchenQueue;

    int option = 0;
    cout << "Pizza Kitchen Display System\n";
    while (option != 3) {
        cout << "\n1 [Order Pizza            ]\n";
        cout << "2 [Kitchen                ]\n";
        cout << "3 [Exit                   ]\n\n";
        cout << "Enter Option: ";
        cin >> option;

        switch (option) {
            case 1:
                if (pizzaCount > 0) {
                    takeOrder(orderQueue, pizzas, pizzaCount);
                    cout << endl;
                } else {
                    cout << "No pizzas available for ordering.\n"<<endl;
                }
                break;
            case 2:
                sendToKitchen(orderQueue, kitchenQueue);
                processQueue(kitchenQueue);
                break;
            case 3:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    }
    return 0;
}

