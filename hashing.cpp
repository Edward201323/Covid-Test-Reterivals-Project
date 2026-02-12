#include "hashing.hpp"

int cs20::hash(const int& key) {
    int hashValue = key % 2147483647; // Use a large prime number to avoid overflow
    if (hashValue < 0) {
        hashValue += 2147483647; // Adjust for negative values
    }
    return hashValue;
}

int cs20::hash(const std::string& key) {
    int hashValue = 0;
    int base = 31; // Use a prime number as the base
    for (char ch : key) {
        hashValue = (hashValue * base + static_cast<unsigned char>(ch)) % 2147483647;
    }
    if (hashValue < 0) {
        hashValue += 2147483647;
    }
    return hashValue;
}

int cs20::hash(const StreetAddress& key) {
    int hashVal = 17;
    hashVal = (hashVal * 31 + cs20::hash(key.number)) % 2147483647;
    hashVal = (hashVal * 31 + cs20::hash(key.street)) % 2147483647;
    hashVal = (hashVal * 31 + cs20::hash(key.city)) % 2147483647;
    hashVal = (hashVal * 31 + cs20::hash(key.zip)) % 2147483647;
    if (hashVal < 0) {
        hashVal += 2147483647;
    }
    return hashVal;
}
