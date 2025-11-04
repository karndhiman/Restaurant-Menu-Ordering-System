#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

class RestaurantOrderingSystem {
private:
    struct MenuItem {
        string name;
        int price;
        string category;
        string code;
    };

    vector<MenuItem> menu;
    unordered_map<string, pair<int, int>> orders; // item_name -> {price, quantity}
    
    void initializeMenu() {
        // Veg Items
        menu.push_back({"Matar Paneer", 150, "veg", "V1"});
        menu.push_back({"Aaloo Paratha", 25, "veg", "V2"});
        menu.push_back({"Butter Roti", 15, "veg", "V3"});
        menu.push_back({"Pulao", 120, "veg", "V4"});
        menu.push_back({"Paneer Roll", 100, "veg", "V5"});
        
        // Non-Veg Items
        menu.push_back({"Butter Chicken", 250, "nonveg", "NV1"});
        menu.push_back({"Boiled Egg", 25, "nonveg", "NV2"});
        menu.push_back({"Omelette(2 eggs)", 80, "nonveg", "NV3"});
        menu.push_back({"Chicken Briyani", 180, "nonveg", "NV4"});
        menu.push_back({"Egg Roll(2 eggs)", 100, "nonveg", "NV5"});
    }

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int getValidatedInput(const string& prompt, int minVal, int maxVal) {
        int input;
        while (true) {
            cout << prompt;
            cin >> input;
            if (cin.fail() || input < minVal || input > maxVal) {
                cout << "Invalid input! Please enter a number between " 
                     << minVal << " and " << maxVal << "." << endl;
                clearInputBuffer();
            } else {
                clearInputBuffer();
                return input;
            }
        }
    }

    void displayMenuByCategory(const string& category) {
        cout << "\n" << string(50, '=') << endl;
        cout << "         " << (category == "veg" ? "VEG ITEMS" : "NON-VEG ITEMS") << endl;
        cout << string(50, '=') << endl;
        cout << left << setw(8) << "Code" << setw(25) << "Item Name" 
             << setw(15) << "Price (Rs.)" << endl;
        cout << string(50, '-') << endl;

        for (const auto& item : menu) {
            if (item.category == category) {
                cout << left << setw(8) << item.code 
                     << setw(25) << item.name 
                     << setw(15) << item.price << endl;
            }
        }
        cout << string(50, '=') << endl;
    }

    void displayCurrentOrders() {
        if (orders.empty()) {
            cout << "\nYour cart is empty!" << endl;
            return;
        }

        cout << "\n" << string(60, '=') << endl;
        cout << "                     CURRENT ORDER" << endl;
        cout << string(60, '=') << endl;
        cout << left << setw(25) << "Item" << setw(10) << "Quantity" 
             << setw(15) << "Price Each" << setw(15) << "Total" << endl;
        cout << string(60, '-') << endl;

        int subtotal = 0;
        for (const auto& order : orders) {
            int itemTotal = order.second.first * order.second.second;
            cout << left << setw(25) << order.first 
                 << setw(10) << order.second.second
                 << setw(15) << order.second.first
                 << setw(15) << itemTotal << endl;
            subtotal += itemTotal;
        }

        cout << string(60, '-') << endl;
        cout << right << setw(50) << "Subtotal: Rs. " << subtotal << endl;
        cout << string(60, '=') << endl;
    }

    void addItemToOrder(const string& itemCode, int quantity) {
        for (const auto& item : menu) {
            if (item.code == itemCode) {
                if (orders.find(item.name) != orders.end()) {
                    // Item already exists, update quantity
                    orders[item.name].second += quantity;
                } else {
                    // New item
                    orders[item.name] = {item.price, quantity};
                }
                cout << "\n✓ Added " << quantity << " x " << item.name << " to your order." << endl;
                return;
            }
        }
        cout << "Item not found!" << endl;
    }

    void removeItemFromOrder() {
        if (orders.empty()) {
            cout << "No items to remove!" << endl;
            return;
        }

        displayCurrentOrders();
        cout << "\nEnter the item name to remove: ";
        string itemName;
        getline(cin, itemName);

        if (orders.find(itemName) != orders.end()) {
            cout << "Enter quantity to remove (0 to remove all): ";
            int quantity;
            cin >> quantity;
            clearInputBuffer();

            if (quantity <= 0 || quantity >= orders[itemName].second) {
                orders.erase(itemName);
                cout << "✓ Removed " << itemName << " from your order." << endl;
            } else {
                orders[itemName].second -= quantity;
                cout << "✓ Reduced quantity of " << itemName << " by " << quantity << "." << endl;
            }
        } else {
            cout << "Item not found in your order!" << endl;
        }
    }

    void processCategoryOrder(const string& category) {
        while (true) {
            displayMenuByCategory(category);
            
            cout << "\nOptions:" << endl;
            cout << "1. Add item to order" << endl;
            cout << "2. View current order" << endl;
            cout << "3. Remove item from order" << endl;
            cout << "4. Back to main menu" << endl;
            
            int choice = getValidatedInput("Enter your choice (1-4): ", 1, 4);

            if (choice == 1) {
                cout << "Enter item code: ";
                string itemCode;
                cin >> itemCode;
                clearInputBuffer();

                // Validate item code
                bool validCode = false;
                for (const auto& item : menu) {
                    if (item.code == itemCode && item.category == category) {
                        validCode = true;
                        break;
                    }
                }

                if (!validCode) {
                    cout << "Invalid item code for this category!" << endl;
                    continue;
                }

                int quantity = getValidatedInput("Enter quantity (1-9): ", 1, 9);
                addItemToOrder(itemCode, quantity);

            } else if (choice == 2) {
                displayCurrentOrders();
            } else if (choice == 3) {
                removeItemFromOrder();
            } else if (choice == 4) {
                break;
            }
        }
    }

    void generateFinalBill() {
        if (orders.empty()) {
            cout << "\nNo orders to generate bill!" << endl;
            return;
        }

        cout << "\n" << string(70, '=') << endl;
        cout << "                      FINAL BILL" << endl;
        cout << string(70, '=') << endl;
        cout << left << setw(25) << "Item" << setw(10) << "Quantity" 
             << setw(15) << "Price Each" << setw(15) << "Total" << endl;
        cout << string(70, '-') << endl;

        int subtotal = 0;
        for (const auto& order : orders) {
            int itemTotal = order.second.first * order.second.second;
            cout << left << setw(25) << order.first 
                 << setw(10) << order.second.second
                 << setw(15) << order.second.first
                 << setw(15) << itemTotal << endl;
            subtotal += itemTotal;
        }

        double tax = subtotal * 0.10; // 10% tax
        double total = subtotal + tax;

        cout << string(70, '-') << endl;
        cout << right << setw(55) << "Subtotal: Rs. " << subtotal << endl;
        cout << right << setw(55) << "Tax (10%): Rs. " << fixed << setprecision(2) << tax << endl;
        cout << right << setw(55) << "Total: Rs. " << fixed << setprecision(2) << total << endl;
        cout << string(70, '=') << endl;
    }

public:
    RestaurantOrderingSystem() {
        initializeMenu();
    }

    void startOrdering() {
        cout << string(60, '=') << endl;
        cout << "       WELCOME TO RESTAURANT ORDERING SYSTEM" << endl;
        cout << string(60, '=') << endl;

        while (true) {
            cout << "\nMAIN MENU:" << endl;
            cout << "1. View Veg Menu" << endl;
            cout << "2. View Non-Veg Menu" << endl;
            cout << "3. View Current Order" << endl;
            cout << "4. Generate Bill & Exit" << endl;
            cout << "5. Exit Without Ordering" << endl;

            int choice = getValidatedInput("Enter your choice (1-5): ", 1, 5);

            switch (choice) {
                case 1:
                    processCategoryOrder("veg");
                    break;
                case 2:
                    processCategoryOrder("nonveg");
                    break;
                case 3:
                    displayCurrentOrders();
                    break;
                case 4:
                    generateFinalBill();
                    cout << "\nThank you for your order! Visit again!" << endl;
                    return;
                case 5:
                    if (!orders.empty()) {
                        cout << "You have items in your cart. Are you sure you want to exit? (y/n): ";
                        char confirm;
                        cin >> confirm;
                        clearInputBuffer();
                        if (confirm == 'y' || confirm == 'Y') {
                            cout << "Thank you for visiting! Hope to see you again!" << endl;
                            return;
                        }
                    } else {
                        cout << "Thank you for visiting! Hope to see you again!" << endl;
                        return;
                    }
                    break;
            }
        }
    }
};

int main() {
    RestaurantOrderingSystem restaurant;
    restaurant.startOrdering();
    return 0;
}
