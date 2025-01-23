#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;
//Log In
//Create account
//Array- User name & Password
class Product {
private:
    long productID;
    string name;
    float price;
    int quantity;
    string category;
    static long nextProductID;

public:
    Product() {}
    Product(string name, float price, int quantity, string category);

    long getProductID() { return productID; }
    string getName() { return name; }
    float getPrice() { return price; }
    int getQuantity() { return quantity; }
    string getCategory() { return category; }

    void updateQuantity(int qty) { quantity += qty; }
    friend ostream &operator<<(ostream &os, Product &p);
};

long Product::nextProductID = 1;

Product::Product(string name, float price, int quantity, string category) {
    this->productID = nextProductID++;
    this->name = name;
    this->price = price;
    this->quantity = quantity;
    this->category = category;
}

ostream &operator<<(ostream &os, Product &p) {
    os << "Product ID: " << p.productID << endl;
    os << "Name: " << p.name << endl;
    os << "Price: " << fixed << setprecision(2) << p.price << endl;
    os << "Quantity: " << p.quantity << endl;
    os << "Category: " << p.category << endl;
    return os;
}

class Order {
private:
    long orderID;
    string customerName;
    vector<pair<long, int>> products; // productID -> quantity
    float totalCost;
    string orderDate;
    static long nextOrderID;

public:
    Order() {}
    Order(string customerName, vector<pair<long, int>> products, float totalCost, string orderDate);

    long getOrderID() { return orderID; }
    string getCustomerName() { return customerName; }
    float getTotalCost() { return totalCost; }
    string getOrderDate() { return orderDate; }

    friend ostream &operator<<(ostream &os, Order &o);
};

long Order::nextOrderID = 1;

Order::Order(string customerName, vector<pair<long, int>> products, float totalCost, string orderDate) {
    this->orderID = nextOrderID++;
    this->customerName = customerName;
    this->products = products;
    this->totalCost = totalCost;
    this->orderDate = orderDate;
}

ostream &operator<<(ostream &os, Order &o) {
    os << "Order ID: " << o.orderID << endl;
    os << "Customer Name: " << o.customerName << endl;
    os << "Total Cost: " << fixed << setprecision(2) << o.totalCost << endl;
    os << "Order Date: " << o.orderDate << endl;
    os << "Products: " << endl;
    for (auto &p : o.products) {
        os << "  Product ID: " << p.first << " Quantity: " << p.second << endl;
    }
    return os;
}

class OrderProcessing {
private:
    vector<Product> products;
    vector<Order> orders;

    void loadPermanentProducts();

public:
    OrderProcessing();
    void addProduct(string name, float price, int quantity, string category);
    void showAllProducts();
    void createOrder(string customerName, vector<pair<long, int>> productQuantities);
    void showAllOrders();
};

OrderProcessing::OrderProcessing() {
    loadPermanentProducts();
}

void OrderProcessing::loadPermanentProducts() {
    products.push_back(Product("Laptop", 999.99, 10, "Electronics"));
    products.push_back(Product("Phone", 499.99, 20, "Electronics"));
    products.push_back(Product("Headphones", 79.99, 50, "Accessories"));
    products.push_back(Product("Bread", 2.99, 100, "Food"));
    products.push_back(Product("Milk", 1.49, 200, "Food"));
    products.push_back(Product("T-shirt", 14.99, 75, "Cloth"));
    products.push_back(Product("Laddu", 1.09, 750, "Food"));
}

void OrderProcessing::addProduct(string name, float price, int quantity, string category) {
    products.push_back(Product(name, price, quantity, category));
    cout << "Product added successfully." << endl;
}

void OrderProcessing::showAllProducts() {
    if (products.empty()) {
        cout << "No products available." << endl;
        return;
    }
    for (auto &p : products) {
        cout << p << endl;
    }
}

void OrderProcessing::createOrder(string customerName, vector<pair<long, int>> productQuantities) {
    float totalCost = 0;
    for (auto &pq : productQuantities) {
        long productID = pq.first;
        int quantity = pq.second;

        bool productFound = false;
        for (auto &product : products) {
            if (product.getProductID() == productID) {
                productFound = true;
                if (product.getQuantity() < quantity) {
                    cout << "Insufficient stock for Product ID " << productID << endl;
                    return;
                }
                totalCost += product.getPrice() * quantity;
                product.updateQuantity(-quantity);
                break;
            }
        }

        if (!productFound) {
            cout << "Product ID " << productID << " not found." << endl;
            return;
        }
    }

    orders.push_back(Order(customerName, productQuantities, totalCost, "2025-01-15")); // Hardcoded date for simplicity
    cout << "Order created successfully." << endl;
    cout << orders.back() << endl;
}

void OrderProcessing::showAllOrders() {
    if (orders.empty()) {
        cout << "No orders placed yet." << endl;
        return;
    }
    for (auto &o : orders) {
        cout << o << endl;
    }
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

bool login(vector<string>& usernames, vector<string>& passwords) {
    string inputUsername, inputPassword;
    cout << "Enter username: ";
    cin >> inputUsername;
    cout << "Enter password: ";
    cin >> inputPassword;

    for (int i = 0; i < usernames.size(); ++i) {
        if (usernames[i] == inputUsername && passwords[i] == inputPassword) {
            cout << "Login successful. Welcome, " << inputUsername << "!" << endl;
            return true;
        }
    }
    cout << "Invalid username or password." << endl;
    return false;
}

void createAccount(vector<string>& usernames, vector<string>& passwords) {
    string newUsername, newPassword;
    cout << "Enter a new username: ";
    cin >> newUsername;
    cout << "Enter a new password: ";
    cin >> newPassword;

    usernames.push_back(newUsername);
    passwords.push_back(newPassword);

    cout << "Account created successfully! Please log in to continue.\n";
}

int main() {
    vector<string> usernames = {"Rabbani", "user1", "user2"};
    vector<string> passwords = {"223071062", "password1", "password2"};
    cout << "Online Selling System" << endl;
    cout << "Do you have an account? (yes/no): ";
    string accountChoice;
    cin >> accountChoice;

    bool loggedIn = false;
    if (accountChoice == "yes" || accountChoice == "Yes") {
        if (!login(usernames, passwords)) {
            cout << "Access denied." << endl;
            return 0;
        } else {
            loggedIn = true;
        }
    } else if (accountChoice == "no" || accountChoice == "No") {
        createAccount(usernames, passwords);
        // Prompt to log in after account creation
        cout << "Please log in with your new account.\n";
        loggedIn = login(usernames, passwords);
    } else {
        cout << "Invalid choice!" << endl;
        return 0;
    }

    if (loggedIn) {
        OrderProcessing op;
        int choice;

        do {
            cout << "\n*** Order Processing System ***\n";
            cout << "1. Add Product\n";
            cout << "2. Show All Products\n";
            cout << "3. Create Order\n";
            cout << "4. Show All Orders\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                string name, category;
                float price;
                int quantity;

                cout << "Enter product name: ";
                cin >> name;
                cout << "Enter price: ";
                cin >> price;
                cout << "Enter quantity: ";
                cin >> quantity;
                cout << "Enter category: ";
                cin >> category;

                op.addProduct(name, price, quantity, category);
                break;
            }
            case 2:
                op.showAllProducts();
                break;
            case 3: {
                string customerName;
                int productCount;
                vector<pair<long, int>> productQuantities;

                cout << "Enter customer name: ";
                cin >> customerName;
                cout << "Enter number of products in the order: ";
                cin >> productCount;

                for (int i = 0; i < productCount; ++i) {
                    long productID;
                    int quantity;

                    cout << "Enter Product ID: ";
                    cin >> productID;
                    cout << "Enter Quantity: ";
                    cin >> quantity;

                    productQuantities.push_back(make_pair(productID, quantity));
                }

                op.createOrder(customerName, productQuantities);
                break;
            }
            case 4:
                op.showAllOrders();
                break;
            case 5:
                cout << "Exiting system." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 5);
    }

    return 0;
}
