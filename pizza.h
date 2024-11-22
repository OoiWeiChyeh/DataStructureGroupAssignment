
#ifndef PIZZA_H
#define PIZZA_H

#include <string>
#include <iostream>
using namespace std;

class Pizza {
public:
    string name;
    float smallPrice, mediumPrice, largePrice, extraLargePrice;

    // Default and parameterized constructor
    Pizza(const string& name = "", float small = 0.0, float medium = 0.0, float large = 0.0, float xl = 0.0)
        : name(name), smallPrice(small), mediumPrice(medium), largePrice(large), extraLargePrice(xl) {}

    // Display pizza details
    void display() const {
        cout << "Pizza: " << name << endl
             << "Small: $" << smallPrice << " Medium: $" << mediumPrice
             << " Large: $" << largePrice << " Extra Large: $" << extraLargePrice << endl;
    }
};

#endif // PIZZA_H

