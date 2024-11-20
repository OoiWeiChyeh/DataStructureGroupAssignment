#include "pizza.h"
#include <iostream>

int main() {
    Pizza pizzas[10];
    int pizzaCount = 0;

    // Load pizzas from file
    loadPizzas(pizzas, pizzaCount, "pizza.txt");

    // Queues for orders and kitchen
    queue<Order> orderQueue;
    queue<Order> kitchenQueue;

    int option = 0;
    while (option != 3) {
        cout << "\n1. [Order Pizza]\n";
        cout << "2. [Kitchen]\n";
        cout << "3. [Exit]\n";
        cout << "Select an option: ";
        cin >> option;

        switch (option) {
            case 1:
                displayMenu(pizzas, pizzaCount);
                takeOrder(orderQueue, pizzas, pizzaCount);
                displayOrderSummary(orderQueue);

                char confirm;
                cout << "Confirm send to the kitchen [Y/N]: ";
                cin >> confirm;
                if (tolower(confirm) == 'y') {
                    sendToKitchen(orderQueue, kitchenQueue);
                    cout << "Order sent to kitchen.\n";
                }
                break;

            case 2:
                if (kitchenQueue.empty()) {
                    cout << "No pizzas in the kitchen queue.\n";
                } else {
                    kitchenQueueMenu(kitchenQueue);
                }
                break;

            case 3:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid input. Try again.\n";
        }
    }

    return 0;
}

