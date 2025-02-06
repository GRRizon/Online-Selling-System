#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
using namespace std;

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
    friend istream &operator>>(istream &is, Product &p);
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
    os << p.productID << "," << p.name << "," << p.price << "," << p.quantity << "," << p.category << endl;
    return os;
}

istream &operator>>(istream &is, Product &p) {
    getline(is, p.name, ',');
    is >> p.price;
    is.ignore();
    is >> p.quantity;
    is.ignore();
    getline(is, p.category);
    p.productID = Product::nextProductID++;
    return is;
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
    friend istream &operator>>(istream &is, Order &o);
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
    os << o.orderID << "," << o.customerName << "," << o.totalCost << "," << o.orderDate << "\n";
    for (auto &p : o.products) {
        os << p.first << "," << p.second << " ";
    }
    os << endl;
    return os;
}

istream &operator>>(istream &is, Order &o) {
    is >> o.orderID;
    is.ignore();
    getline(is, o.customerName, ',');
    is >> o.totalCost;
    is.ignore();
    getline(is, o.orderDate);
    string productData;
    getline(is, productData);
    stringstream ss(productData);
    long productID;
    int qty;
    while (ss >> productID >> qty) {
        o.products.push_back(make_pair(productID, qty));
    }
    return is;
}

class OrderProcessing {
private:
    vector<Product> products;
    vector<Order> orders;
    vector<string> usernames;
    vector<string> passwords;

    void loadPermanentProducts();
    void loadOrders();
    void loadUserAccounts();
    void savePermanentProducts();
    void saveOrders();
    void saveUserAccounts();

public:
    OrderProcessing();
    void addProduct(string name, float price, int quantity, string category);
    void showAllProducts();
    void createOrder(string customerName, vector<pair<long, int>> productQuantities);
    void showAllOrders();
    bool login();
    void createAccount();
};

OrderProcessing::OrderProcessing() {
    loadPermanentProducts();
    loadOrders();
    loadUserAccounts();
}

void OrderProcessing::loadPermanentProducts() {
    ifstream inFile("products.txt");
    Product p;
    while (inFile >> p) {
        products.push_back(p);
    }
}

void OrderProcessing::loadOrders() {
    ifstream inFile("orders.txt");
    Order o;
    while (inFile >> o) {
        orders.push_back(o);
    }
}

void OrderProcessing::loadUserAccounts() {
    ifstream inFile("users.txt");
    string username, password;
    while (inFile >> username >> password) {
        usernames.push_back(username);
        passwords.push_back(password);
    }
}

void OrderProcessing::savePermanentProducts() {
    ofstream outFile("products.txt");
    for (auto &p : products) {
        outFile << p;
    }
}

void OrderProcessing::saveOrders() {
    ofstream outFile("orders.txt");
    for (auto &o : orders) {
        outFile << o;
    }
}

void OrderProcessing::saveUserAccounts() {
    ofstream outFile("users.txt");
    for (size_t i = 0; i < usernames.size(); ++i) {
        outFile << usernames[i] << " " << passwords[i] << endl;
    }
}

void OrderProcessing::addProduct(string name, float price, int quantity, string category) {
    products.push_back(Product(name, price, quantity, category));
    savePermanentProducts();
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

    orders.push_back(Order(customerName, productQuantities, totalCost, "2025-01-15"));
    saveOrders();
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

bool OrderProcessing::login() {
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

void OrderProcessing::createAccount() {
    string newUsername, newPassword;
    cout << "Enter a new username: ";
    cin >> newUsername;
    cout << "Enter a new password: ";
    cin >> newPassword;

    usernames.push_back(newUsername);
    passwords.push_back(newPassword);
    saveUserAccounts();
    cout << "Account created successfully! Please log in to continue.\n";
}

int main() {
    OrderProcessing op;
    int choice;
    bool loggedIn = false;

    cout << "Online Selling System" << endl;
    cout << "Do you have an account? (yes/no): ";
    string accountChoice;
    cin >> accountChoice;

    if (accountChoice == "yes" || accountChoice == "Yes") {
        if (!op.login()) {
            cout << "Access denied." << endl;
            return 0;
        } else {
            loggedIn = true;
        }
    } else if (accountChoice == "no" || accountChoice == "No") {
        op.createAccount();
        cout << "Please log in with your new account.\n";
        loggedIn = op.login();
    } else {
        cout << "Invalid choice!" << endl;
        return 0;
    }

    if (loggedIn) {
        do {
            cout << "\n** Order Processing System **\n";
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
