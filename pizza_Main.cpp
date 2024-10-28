#include "pizza.h"
#include <iomanip>
// Function prototypes
void loadPizzas(Pizza pizzas[], int& count);
void displayMenu(const Pizza pizzas[], int count);
void takeOrder(Queue<string>& orderQueue, const Pizza pizzas[], int count);
void displayOrders(const Queue<Order>& orderQueue);
void sendToKitchen(Queue<string>& orderQueue, Queue<string>& kitchenQueue);
void processQueue(Queue<string>& kitchenQueue);

int main() {
    Pizza pizzas[10];
    int pizzaCount, orderNum = 1;

    // Load pizzas from file
    loadPizzas(pizzas, pizzaCount);


    // Queue for storing orders
    Queue<string> orderQueue;
    Queue<string> kitchenQueue;
		
		cout << "Welcome to Pizza Kitchen \n";
    int option = 0;
    while (option != 4) {
        cout << left << "\n1.  [Order Pizza" << right << setw(17) << " ]" ;
		cout << left << "\n2.  [Send to Kitchen" << right << setw(13) << " ]";
		cout << left << "\n3.  [Process with Kitchen Queue" << right << setw(2) << " ]";
		cout << left << "\n4.  [Exit" << right << setw(26) << " ]\n\n";
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
