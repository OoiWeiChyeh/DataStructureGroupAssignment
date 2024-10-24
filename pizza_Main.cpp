#include "pizza.h"

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

    // Queue for storing orders
    Queue<string> orderQueue;
    Queue<string> kitchenQueue;

    int option = 0;
    while (option != 4) {
        cout << "\n1. Order Pizza\n2. Send to Kitchen\n3. Process Kitchen Queue\n4. Exit\n";
        cout << "Select an option: ";
        cin >> option;

        switch (option) {
            case 1:
                takeOrder(orderQueue, pizzas, pizzaCount);
                break;
            case 2:
                sendToKitchen(orderQueue, kitchenQueue);
                break;
            case 3:
                processQueue(kitchenQueue);
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    }

    return 0;
}

