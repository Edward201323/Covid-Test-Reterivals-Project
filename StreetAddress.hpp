#pragma once

#include <string>

// A struct representing a (simplified) street address
struct StreetAddress {
    int number;
    std::string street;
    std::string city;
    int zip;

    bool operator==(const StreetAddress& other) const {
        return number == other.number &&
               street == other.street &&
               city == other.city &&
               zip == other.zip;
    }
};