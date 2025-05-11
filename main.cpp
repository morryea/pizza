#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <iomanip>
#include <algorithm>

using namespace std;

// ----------- MenuItem -----------
class MenuItem {
protected:
    string name;
    double basePrice;

public:
    MenuItem(string n, double price) : name(n), basePrice(price) {}
    virtual ~MenuItem() = default;

    string getName() const { return name; }
    double getBasePrice() const { return basePrice; }

    virtual double calculatePrice() const = 0;
    virtual void display() const = 0;
};

// ----------- Topping -----------
struct Topping {
    string name;
    double price;
    Topping(string n, double p) : name(n), price(p) {}
};

// ----------- Pizza -----------
class Pizza : public MenuItem {
public:
    enum class Size { SMALL, MEDIUM, LARGE };
    enum class BaseType { THIN, TRADITIONAL, THICK };

private:
    Size size;
    BaseType baseType;
    vector<Topping> toppings;

public:
    Pizza(string n, double price) : MenuItem(n, price), size(Size::MEDIUM), baseType(BaseType::TRADITIONAL) {}

    void setSize(Size s) { size = s; }
    void setBaseType(BaseType bt) { baseType = bt; }
    void addTopping(const Topping& t) { toppings.push_back(t); }

    void removeTopping(const string& toppingName) {
        toppings.erase(remove_if(toppings.begin(), toppings.end(),
                                 [&](const Topping& t) { return t.name == toppingName; }),
                       toppings.end());
    }

    double calculatePrice() const override {
        double finalPrice = basePrice;
        if (size == Size::LARGE) finalPrice += 3;
        else if (size == Size::SMALL) finalPrice -= 2;
        for (const auto& t : toppings) finalPrice += t.price;
        return finalPrice;
    }

    void display() const override {
        cout << "Pizza: " << name << " | Size: ";
        switch (size) {
            case Size::SMALL: cout << "Small"; break;
            case Size::MEDIUM: cout << "Medium"; break;
            case Size::LARGE: cout << "Large"; break;
        }
        cout << " | Base: ";
        switch (baseType) {
            case BaseType::THIN: cout << "Thin"; break;
            case BaseType::TRADITIONAL: cout << "Traditional"; break;
            case BaseType::THICK: cout << "Thick"; break;
        }
        cout << " | Price: $" << fixed << setprecision(2) << calculatePrice() << endl;
        if (!toppings.empty()) {
            cout << "Toppings: ";
            for (const auto& t : toppings)
                cout << t.name << " ";
            cout << endl;
        }
    }
};

// ----------- Drink -----------
class Drink : public MenuItem {
private:
    double volume;
    bool isCarbonated;

public:
    Drink(string n, double price, double v, bool carbonated)
        : MenuItem(n, price), volume(v), isCarbonated(carbonated) {}

    double calculatePrice() const override {
        return basePrice + (volume > 0.5 ? 1.0 : 0.0);
    }

    void display() const override {
        cout << "Drink: " << name << " | Volume: " << volume << "L | "
             << (isCarbonated ? "Carbonated" : "Still") << " | Price: $"
             << fixed << setprecision(2) << calculatePrice() << endl;
    }
};

// ----------- SideDish -----------
class SideDish : public MenuItem {
private:
    string portionSize;

public:
    SideDish(string n, double price, string portion)
        : MenuItem(n, price), portionSize(portion) {}

    double calculatePrice() const override {
        return basePrice + (portionSize == "Large" ? 1.5 : 0.0);
    }

    void display() const override {
        cout << "Side Dish: " << name << " | Portion: " << portionSize << " | Price: $"
             << fixed << setprecision(2) << calculatePrice() << endl;
    }
};

// ----------- Menu -----------
class Menu {
private:
    vector<Pizza> availablePizzas;
    vector<Drink> availableDrinks;
    vector<SideDish> availableSideDishes;
    vector<Topping> availableToppings;

public:
    Menu() {
        availablePizzas.emplace_back("Margherita", 6.0);
        availablePizzas.emplace_back("Pepperoni", 7.5);
        availablePizzas.emplace_back("Vegetarian", 7.0);

        availableDrinks.emplace_back("Cola", 2.0, 0.5, true);
        availableDrinks.emplace_back("Juice", 2.5, 0.3, false);

        availableSideDishes.emplace_back("Fries", 3.0, "Medium");
        availableSideDishes.emplace_back("Nuggets", 4.0, "Large");

        availableToppings.emplace_back("Mushrooms", 0.5);
        availableToppings.emplace_back("Olives", 0.4);
        availableToppings.emplace_back("Bacon", 0.8);
    }

    void displayMenu() const {
        cout << "--- Pizzas ---" << endl;
        for (size_t i = 0; i < availablePizzas.size(); ++i)
            cout << "P" << i + 1 << ". " << availablePizzas[i].getName() << " ($" << availablePizzas[i].getBasePrice() << ")\n";

        cout << "--- Drinks ---" << endl;
        for (size_t i = 0; i < availableDrinks.size(); ++i)
            cout << "D" << i + 1 << ". " << availableDrinks[i].getName() << " ($" << availableDrinks[i].getBasePrice() << ")\n";

        cout << "--- Side Dishes ---" << endl;
        for (size_t i = 0; i < availableSideDishes.size(); ++i)
            cout << "S" << i + 1 << ". " << availableSideDishes[i].getName() << " ($" << availableSideDishes[i].getBasePrice() << ")\n";
    }

    void displayAvailableToppings() const {
        cout << "--- Available Toppings ---" << endl;
        for (size_t i = 0; i < availableToppings.size(); ++i)
            cout << i + 1 << ". " << availableToppings[i].name << " ($" << availableToppings[i].price << ")\n";
    }

    Pizza getPizzaCopy(int index) { return availablePizzas.at(index); }
    Drink getDrinkCopy(int index) { return availableDrinks.at(index); }
    SideDish getSideDishCopy(int index) { return availableSideDishes.at(index); }
    Topping getToppingCopy(int index) const { return availableToppings.at(index); }
};

// ----------- Order -----------
class Order {
public:
    enum class OrderType { DINE_IN, TAKEAWAY, DELIVERY };
    enum class OrderStatus { PENDING, PREPARING, READY, DELIVERED, CANCELLED };

private:
    int orderId;
    vector<Pizza> orderedPizzas;
    vector<Drink> orderedDrinks;
    vector<SideDish> orderedSideDishes;
    string customerName;
    OrderType orderType;
    OrderStatus status;
    string deliveryAddress;
    double deliveryFee = 0.0;
    double totalAmount = 0.0;

public:
    Order(int id) : orderId(id), status(OrderStatus::PENDING) {}

    void addPizza(const Pizza& p) { orderedPizzas.push_back(p); }
    void addDrink(const Drink& d) { orderedDrinks.push_back(d); }
    void addSideDish(const SideDish& s) { orderedSideDishes.push_back(s); }

    void setCustomerName(const string& name) { customerName = name; }
    void setOrderType(OrderType type) { orderType = type; }
    void setDeliveryAddress(const string& addr) { deliveryAddress = addr; }
    void updateStatus(OrderStatus newStatus) { status = newStatus; }

    void calculateTotal() {
        totalAmount = 0;
        for (const auto& p : orderedPizzas) totalAmount += p.calculatePrice();
        for (const auto& d : orderedDrinks) totalAmount += d.calculatePrice();
        for (const auto& s : orderedSideDishes) totalAmount += s.calculatePrice();
        if (orderType == OrderType::DELIVERY) deliveryFee = 3.0;
        totalAmount += deliveryFee;
    }

    void displayOrder() const {
        cout << "Order #" << orderId << " for " << customerName << endl;
        for (const auto& p : orderedPizzas) p.display();
        for (const auto& d : orderedDrinks) d.display();
        for (const auto& s : orderedSideDishes) s.display();
        if (orderType == OrderType::DELIVERY)
            cout << "Delivery Address: " << deliveryAddress << " | Fee: $" << deliveryFee << endl;
        cout << "Total: $" << totalAmount << endl;
    }
};

// ----------- Pizzeria + main() -----------
class Pizzeria {
private:
    Menu menu;
    vector<Order> activeOrders;
    int nextOrderId = 1;

public:
    void run() {
        while (true) {
            cout << "\n1. Create Order\n2. View Menu\n3. Exit\nChoice: ";
            int choice;
            cin >> choice;

            if (choice == 1) takeNewOrder();
            else if (choice == 2) menu.displayMenu();
            else break;
        }
    }

    void customizePizza(Pizza& pizza) {
        menu.displayAvailableToppings();
        cout << "Enter topping index to add (-1 to stop): ";
        int idx;
        while (cin >> idx && idx != -1) {
            pizza.addTopping(menu.getToppingCopy(idx - 1));
            cout << "Add another topping or -1 to finish: ";
        }
    }

    void takeNewOrder() {
        Order order(nextOrderId++);
        string name;
        int type;

        cout << "Customer name: ";
        cin.ignore();
        getline(cin, name);
        order.setCustomerName(name);

        cout << "Order Type (0 - Dine In, 1 - Takeaway, 2 - Delivery): ";
        cin >> type;
        order.setOrderType(static_cast<Order::OrderType>(type));
        if (type == 2) {
            string addr;
            cout << "Delivery Address: ";
            cin.ignore();
            getline(cin, addr);
            order.setDeliveryAddress(addr);
        }

        while (true) {
            menu.displayMenu();
            cout << "Select item category: P - Pizza, D - Drink, S - Side Dish, X - Finish: ";
            char c;
            cin >> c;
            if (c == 'X' || c == 'x') break;

            int idx;
            cout << "Enter index (starting from 1): ";
            cin >> idx;

            if (c == 'P' || c == 'p') {
                Pizza p = menu.getPizzaCopy(idx - 1);
                customizePizza(p);
                order.addPizza(p);
            } else if (c == 'D' || c == 'd') {
                order.addDrink(menu.getDrinkCopy(idx - 1));
            } else if (c == 'S' || c == 's') {
                order.addSideDish(menu.getSideDishCopy(idx - 1));
            }
        }

        order.calculateTotal();
        order.displayOrder();
        activeOrders.push_back(order);
    }
};

int main() {
    Pizzeria pizzeria;
    pizzeria.run();
    return 0;
}
