#include "COVIDTestOrder.hpp"
#include <sstream>

COVIDTestOrder::COVIDTestOrder(const std::string& csvLine) {
    std::string temp;
    std::stringstream ss(csvLine);
    getline(ss, temp, ',');
    sa.number = std::stoi(temp);
    getline(ss, sa.street, ',');
    getline(ss, sa.city, ',');
    getline(ss, temp, ',');
    sa.zip = std::stoi(temp);
    getline(ss, temp, ',');
    numOrdered = std::stoi(temp);
}
