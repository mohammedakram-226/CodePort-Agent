#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

struct Order {
    std::string customer;
    std::string product;
    int quantity;
    double unit_price;
    double discount;

    Order(const std::string& c, const std::string& p, int q, double up, double d = 0.0)
        : customer(c), product(p), quantity(q), unit_price(up), discount(d) {}

    double total() const {
        double subtotal = static_cast<double>(quantity) * unit_price;
        return subtotal - (subtotal * discount);
    }
};

static long double round_half_even_scaled(long double x, int ndigits) {
    long double scale = 1.0L;
    for (int i = 0; i < ndigits; ++i) scale *= 10.0L;
    long double y = x * scale;
    long double r = nearbyintl(y);
    return r / scale;
}

static std::string format_python_like(double value, int ndigits) {
    long double r = round_half_even_scaled(static_cast<long double>(value), ndigits);
    std::ostringstream oss;
    oss.setf(std::ios::fixed);
    oss << std::setprecision(ndigits) << static_cast<double>(r);
    std::string s = oss.str();
    std::size_t pos = s.find('.');
    if (pos != std::string::npos) {
        std::size_t end = s.size();
        while (end > pos + 1 && s[end - 1] == '0') --end;
        s.erase(end);
        if (!s.empty() && s.back() == '.') s.push_back('0');
    }
    return s;
}

int main() {
    std::vector<Order> orders = {
        Order("Mohammed", "Laptop", 1, 850.0, 0.10),
        Order("Ahmad", "Mouse", 3, 12.5, 0.0),
        Order("Sara", "Keyboard", 2, 45.0, 0.05),
        Order("Mohammed", "Monitor", 2, 160.0, 0.15),
        Order("Ahmad", "USB Cable", 5, 4.0, 0.0),
        Order("Lina", "Headset", 1, 75.0, 0.20),
    };

    std::unordered_map<std::string, double> customer_totals;
    std::vector<std::string> insertion_order;

    for (const auto& order : orders) {
        if (customer_totals.find(order.customer) == customer_totals.end()) {
            customer_totals[order.customer] = 0.0;
            insertion_order.push_back(order.customer);
        }
        customer_totals[order.customer] += order.total();
    }

    struct Rec {
        std::string name;
        double total;
        std::size_t idx;
    };

    std::vector<Rec> sorted_customers;
    sorted_customers.reserve(insertion_order.size());
    for (std::size_t i = 0; i < insertion_order.size(); ++i) {
        const auto& name = insertion_order[i];
        sorted_customers.push_back({name, customer_totals[name], i});
    }

    std::stable_sort(sorted_customers.begin(), sorted_customers.end(),
        [](const Rec& a, const Rec& b) {
            return a.total > b.total;
        }
    );

    std::cout << "Customer totals:\n";
    for (const auto& rec : sorted_customers) {
        std::cout << rec.name << ": " << format_python_like(rec.total, 2) << "\n";
    }

    double grand_total = 0.0;
    for (const auto& kv : customer_totals) {
        grand_total += kv.second;
    }
    double average_order_value = grand_total / static_cast<double>(orders.size());

    std::cout << "Grand total: " << format_python_like(grand_total, 2) << "\n";
    std::cout << "Average order value: " << format_python_like(average_order_value, 2) << "\n";

    return 0;
}