#pragma once

template<typename Key, typename Val>
class Dictionary {
public:
    // Default constructor
    Dictionary() {}

    // Destructor
    virtual ~Dictionary() {}

    // Remove all records in the dictionary, resetting to the initial state
    virtual void clear() = 0;

    // Retrieve the record that matches the argument key
    virtual Val find(const Key&) const = 0;

    // Add the record as a key-value pair to the dictionary
    // If the key already exists, update the value
    virtual void insert(const Key&, const Val&) = 0;

    // Remove the record that matches the argument key from the dictionary
    virtual void remove(const Key&) = 0;

    // Return the number of records in the dictionary
    virtual int size() const = 0;
};