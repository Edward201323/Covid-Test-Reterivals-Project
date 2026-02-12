#pragma once

#include <vector>
#include "COVIDTestOrder.hpp"
#include "Dictionary.hpp"

void runSimulator(const std::vector<COVIDTestOrder>& orders, Dictionary<StreetAddress, int>* dict, bool analyze);