#pragma once

#include "StreetAddress.hpp"
#include <string>

// A struct representing an incoming order for more test kits
// from a particular household
struct COVIDTestOrder {
    StreetAddress sa;
    int numOrdered; // how many kits they're requesting

    COVIDTestOrder(const StreetAddress &s = {}, int n = 0) : sa(s), numOrdered(n) {}

    // Overloaded constructor:
    // takes a string representing one line in data/orders100k.csv and parses it
    COVIDTestOrder(const std::string &csvLine);
};
