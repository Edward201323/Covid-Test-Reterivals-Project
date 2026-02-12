#include "Simulator.hpp"
#include <iostream>
#include <stdexcept>

void runSimulator(const std::vector<COVIDTestOrder>& orders, Dictionary<StreetAddress, int>* dict, bool analyze) {
    const int MAX_KITS_PER_ADDRESS = 4;
    int orderNum = 1;

    for (const auto& order : orders) {
        const StreetAddress& addr = order.sa;
        int numOrdered = order.numOrdered;
        int totalOrdered = 0;
        bool accept = false;

        try {
            // check for previous orders at the same address
            int previousOrdered = dict->find(addr);
            totalOrdered = previousOrdered + numOrdered;
            if (totalOrdered <= MAX_KITS_PER_ADDRESS) {
                accept = true;
                dict->insert(addr, totalOrdered); // update value if order is accepted
            } else {
                totalOrdered = previousOrdered; // keep previous total if limit exceeded
            }
        } catch (const std::exception& e) {
            // key not found, so this is the first order for this address
            if (numOrdered <= MAX_KITS_PER_ADDRESS) {
                accept = true;
                totalOrdered = numOrdered;
                dict->insert(addr, totalOrdered); // add first-time order
            } else {
                totalOrdered = 0; // too many kits ordered initially, no insertion
            }
        }

        // if analyze mode is on, print the result of each order processing
        if (analyze) {
            if (accept) {
                // print accepted order details
                std::cout << "#" << orderNum << " accepted: " << numOrdered << " kits to "
                          << addr.number << " " << addr.street << ", "
                          << addr.city << " " << addr.zip << " (" << totalOrdered << " total)" << std::endl;
            } else {
                // print rejected order details
                std::cout << "#" << orderNum << " rejected: " << numOrdered << " kits to "
                          << addr.number << " " << addr.street << ", "
                          << addr.city << " " << addr.zip << " (" << totalOrdered << " already)" << std::endl;
            }
        }
        ++orderNum; // increment order number for tracking each order in sequence
    }
}
