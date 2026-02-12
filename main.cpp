#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

#include "COVIDTestOrder.hpp"
#include "UnsortedArrayDictionary.hpp"
#include "HashTableClosed.hpp"
#include "HashTableOpened.hpp"
#include "Simulator.hpp"
#include "Timer.hpp"
#include "hashing.hpp"

// function prototypes for running tests and the simulator loop
void runTests();
void runSimulatorLoop(const std::vector<COVIDTestOrder>& orders, bool analyze);

using std::cout;
using std::endl;

int main() {
    // prompt the user to choose to run either unit tests or the main simulator
    cout << "Enter 'test' to run unit tests or 'run' to execute the simulator: ";
    std::string choice;
    std::cin >> choice;

    if (choice == "test") {
        // run the unit tests if the user chose 'test'
        runTests();
    } else if (choice == "run") {
        // load orders from the csv file for the simulator
        cout << "Loading orders from data/orders100k.csv..." << endl;
        auto startTime = std::chrono::high_resolution_clock::now();

        std::vector<COVIDTestOrder> orders;
        std::ifstream infile("data/orders100k.csv");
        if (!infile) {
            std::cerr << "Failed to open data/orders100k.csv" << endl;
            return 1;
        }

        std::string line;
        // read each line from the csv file and parse it into COVIDTestOrder objects
        while (std::getline(infile, line)) {
            if (!line.empty()) {
                orders.emplace_back(line);
            }
        }
        infile.close();

        // display the time taken to load the orders and the total number loaded
        auto endTime = std::chrono::high_resolution_clock::now();
        auto loadingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        cout << "Finished loading orders. Total orders read: " << orders.size()
             << " in " << loadingDuration << " ms." << endl << endl;

        // ask if the user wants analyze output (note: analyze mode affects timing)
        cout << "Enable analyze output? (This will result in inaccurate time complexity, purely for debugging purposes) (yes/no): ";
        std::string analyzeInput;
        std::cin >> analyzeInput;
        bool analyze = (analyzeInput == "yes" || analyzeInput == "Yes" || analyzeInput == "y" || analyzeInput == "Y");

        // run the main simulator loop
        runSimulatorLoop(orders, analyze);
    } else {
        std::cerr << "Invalid choice." << endl;
    }

    cout << "Program exited." << endl;
    return 0;
}

// function to run the main simulator loop, allowing the user to select options and run simulations
void runSimulatorLoop(const std::vector<COVIDTestOrder>& orders, bool analyze) {
    while (true) {
        // prompt the user to enter the number of orders to process or 'x' to exit
        cout << "Enter number of orders to process (or 'x' to exit): ";
        std::string input;
        std::cin >> input;
        if (input == "x" || input == "X") {
            break; // exit the loop if the user inputs 'x'
        }

        int M;
        // convert the user input to an integer and handle any input errors
        try {
            M = std::stoi(input);
            if (M <= 0) {
                std::cerr << "Please enter a positive integer for the number of orders." << endl;
                continue;
            }
        } catch (const std::exception&) {
            std::cerr << "Invalid input. Please enter a positive integer or 'x' to exit." << endl;
            continue;
        }

        // ensure the number of orders requested does not exceed the available orders
        if (M > static_cast<int>(orders.size())) {
            std::cerr << "Number of orders requested exceeds total orders available ("
                      << orders.size() << ")." << endl;
            continue;
        }

        // prompt the user to select which data structure to use for the simulation
        cout << "Choose data structure (1 for UnsortedArrayDictionary, 2 for HashTableClosed, 3 for HashTableOpened): ";
        std::string dsInput;
        std::cin >> dsInput;
        int dsChoice;
        try {
            dsChoice = std::stoi(dsInput);
            if (dsChoice != 1 && dsChoice != 2 && dsChoice != 3) {
                std::cerr << "Invalid choice. Please enter 1, 2, or 3." << endl;
                continue;
            }
        } catch (const std::exception&) {
            std::cerr << "Invalid input. Please enter 1, 2, or 3." << endl;
            continue;
        }

        // create a subset of orders to process based on user-specified M
        std::vector<COVIDTestOrder> currentOrders(orders.begin(), orders.begin() + M);

        Timer timer;
        int elapsed;

        // run the simulation using the selected data structure
        try {
            if (dsChoice == 1) {
                // using UnsortedArrayDictionary
                cout << "Running with UnsortedArrayDictionary..." << endl;
                UnsortedArrayDictionary<StreetAddress, int> unsortedDict(4 * M); // array dictionary size is 4 * M
                timer.start();
                runSimulator(currentOrders, &unsortedDict, analyze);
                timer.stop();
                elapsed = timer.read();
                cout << "UnsortedArrayDictionary with " << M << " orders took " << elapsed << " ms" << endl << endl;
            } else if (dsChoice == 2) {
                // using HashTableClosed
                cout << "Running with HashTableClosed..." << endl;
                HashTableClosed<StreetAddress, int> hashDict(4 * M); // hash table size is 4 * M
                timer.start();
                runSimulator(currentOrders, &hashDict, analyze);
                timer.stop();
                elapsed = timer.read();
                cout << "HashTableClosed with " << M << " orders took " << elapsed << " ms" << endl << endl;
            } else if (dsChoice == 3) {
                // using HashTableOpened
                cout << "Running with HashTableOpened..." << endl;
                HashTableOpened<StreetAddress, int> hashDict(4 * M); // hash table size is 4 * M
                timer.start();
                runSimulator(currentOrders, &hashDict, analyze);
                timer.stop();
                elapsed = timer.read();
                cout << "HashTableOpened with " << M << " orders took " << elapsed << " ms" << endl << endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "An error occurred during simulation: " << e.what() << endl;
        }
    }
}

// function to run unit tests on the HashTableClosed data structure
void runTests() {
    cout << "Running unit tests on HashTableClosed..." << endl;

    HashTableClosed<int, std::string> hashTable(10);

    // test inserting values into the hash table
    try {
        hashTable.insert(1, "One");
        hashTable.insert(2, "Two");
        hashTable.insert(3, "Three");
        cout << "Insert test passed." << endl;
    } catch (const std::exception& e) {
        std::cerr << "Insert test failed: " << e.what() << endl;
    }

    // test finding a value in the hash table
    try {
        std::string value = hashTable.find(2);
        if (value == "Two") {
            cout << "Find test passed." << endl;
        } else {
            std::cerr << "Find test failed: incorrect value." << endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Find test failed: " << e.what() << endl;
    }

    // test removing a value from the hash table
    try {
        hashTable.remove(2);
        try {
            hashTable.find(2);
            std::cerr << "Remove test failed: key still found after removal." << endl;
        } catch (const std::exception&) {
            cout << "Remove test passed." << endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Remove test failed: " << e.what() << endl;
    }

    // test updating an existing key's value in the hash table
    try {
        hashTable.insert(1, "Uno");
        std::string value = hashTable.find(1);
        if (value == "Uno") {
            cout << "Update existing key test passed." << endl;
        } else {
            std::cerr << "Update existing key test failed: incorrect value." << endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Update existing key test failed: " << e.what() << endl;
    }

    // test inserting values until the hash table is full
    try {
        hashTable.insert(4, "Four");
        hashTable.insert(5, "Five");
        hashTable.insert(6, "Six");
        hashTable.insert(7, "Seven");
        hashTable.insert(8, "Eight");
        hashTable.insert(9, "Nine");
        hashTable.insert(10, "Ten");
        hashTable.insert(11, "Eleven"); // should cause an exception as table is full
        std::cerr << "Insert until full test failed: no exception thrown when table is full." << endl;
    } catch (const std::exception& e) {
        cout << "Insert until full test passed: " << e.what() << endl;
    }

    // optionally, print the hash table contents for verification
    cout << "\nCurrent hash table contents:" << endl;
    hashTable.print();

    cout << "Unit tests completed." << endl << endl;
}
