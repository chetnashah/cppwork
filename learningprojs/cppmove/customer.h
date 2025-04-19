//
// Created by jay shah on 10/04/25.
//

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

class Customer {
private:
    std::string name;
    std::vector<int> values;
public:
    explicit Customer(const std::string&n): name(n) {
        std::cout << "Customer " << n << " created" << std::endl;
        assert(!name.empty());
    }

    std::string get_name() {
        return name;
    }

    void addValue(const int& value) {
        values.push_back(value);
    }

    friend std::ostream& operator<<(std::ostream& os, const Customer& customer) {
        os << "Customer Name: " << customer.name << "\n";
        for (const auto& value : customer.values) {
            os << value << " ";
        }
        return os;
    }

    Customer(const Customer& cust): name(cust.name), values(cust.values) {
        std::cout << "Copy constructor called" << cust.name << std::endl;
    }

    Customer(Customer&& cust) noexcept : name(std::move(cust.name)), values(std::move(cust.values)) {
        std::cout << "Move constructor called" << name << std::endl;
    }

    Customer& operator= (const Customer& cust) {
        std::cout << "Copy assignment operator called" << cust.name << std::endl;
        name = cust.name;
        values = cust.values;
        return *this;
    }

    Customer& operator= (Customer&& cust) noexcept {
        std::cout << "Move assignment operator called" << name << std::endl;
        if (this != &cust) {
            name = std::move(cust.name);
            values = std::move(cust.values);
        }
        return *this;
    }


};