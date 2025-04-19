//
// Created by jay shah on 19/04/25.
//

#pragma once

#include <iostream>

class Shape {
public:
    // virtual destructor
    // essential for derived class cleanup
    virtual ~Shape() {
        std::cout << "Shape destructor called" << std::endl;
    }

    // delete copy constructor
    Shape(const Shape& other) = delete;
    // delete copy assignment operator
    Shape& operator=(const Shape& other) = delete;

    // move ops delete
    Shape(const Shape&& other) = delete;
    Shape& operator=(const Shape&& other) = delete;

    // Default constructor
    Shape() = default;

    // Example virtual function
    virtual void draw() const = 0; // Pure virtual makes Shape abstract

protected:
    std::string color = "transparent";
};