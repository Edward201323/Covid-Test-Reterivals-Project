#pragma once

#include "StreetAddress.hpp"
#include <string>

namespace cs20 {
    int hash(const int& key);
    int hash(const std::string& key);
    int hash(const StreetAddress& key);
}
