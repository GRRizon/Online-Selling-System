// Online-Selling-System

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
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
    map<long, int> products; // productID -> quantity
    float totalCost;
    string orderDate;
    static long nextOrderID;

public:
    Order() {}
    Order(string customerName, map<long, int> products, float totalCost, string orderDate);

    long getOrderID() { return orderID; }
    string getCustomerName() { return customerName; }
    float getTotalCost() { return totalCost; }
    string getOrderDate() { return orderDate; }

    friend ostream &operator<<(ostream &os, Order &o);
};

long Order::nextOrderID = 1;

Order::Order(string customerName, map<long, int> products, float totalCost, string orderDate) {
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
    map<long, Product> products;
    map<long, Order> orders;

    void loadPermanentProducts();

public:
    OrderProcessing();
    void addProduct(string name, float price, int quantity, string category);
    void showAllProducts();
    void createOrder(string customerName, map<long, int> productQuantities);
    void showAllOrders();
};

OrderProcessing::OrderProcessing() {
    loadPermanentProducts();
}

void OrderProcessing::loadPermanentProducts() {
    products[1] = Product("Laptop", 999.99, 10, "Electronics");
    products[2] = Product("Phone", 499.99, 20, "Electronics");
    products[3] = Product("Headphones", 79.99, 50, "Accessories");
    products[4] = Product("Bread", 2.99, 100, "Food");
    products[5] = Product("Milk", 1.49, 200, "Food");
    products[6] = Product("T-shirt", 14.99, 75, "Cloth");
}

void OrderProcessing::addProduct(string name, float price, int quantity, string category) {
    Product p(name, price, quantity, category);
    products[p.getProductID()] = p;
    cout << "Product added successfully." << endl;
}

void OrderProcessing::showAllProducts() {
    if (products.empty()) {
        cout << "No products available." << endl;
        return;
    }
    for (auto &p : products) {
        cout << p.second << endl;
    }
}

void OrderProcessing::createOrder(string customerName, map<long, int> productQuantities) {
    float totalCost = 0;
    for (auto &pq : productQuantities) {
        long productID = pq.first;
        int quantity = pq.second;

        if (products.find(productID) == products.end()) {
            cout << "Product ID " << productID << " not found." << endl;
            return;
        }

        if (products[productID].getQuantity() < quantity) {
            cout << "Insufficient stock for Product ID " << productID << endl;
            return;
        }

        totalCost += products[productID].getPrice() * quantity;
        products[productID].updateQuantity(-quantity);
    }

    Order o(customerName, productQuantities, totalCost, "2025-01-15"); // Hardcoded date for simplicity
    orders[o.getOrderID()] = o;
    cout << "Order created successfully." << endl;
    cout << o << endl;
}

void OrderProcessing::showAllOrders() {
    if (orders.empty()) {
        cout << "No orders placed yet." << endl;
        return;
    }
    for (auto &o : orders) {
        cout << o.second << endl;
    }
}

int main() {
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
            map<long, int> productQuantities;

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

                productQuantities[productID] = quantity;
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

    return 0;
}
