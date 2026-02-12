#pragma once

#include "Dictionary.hpp"
#include <stdexcept>

template<typename Key, typename Val>
class UnsortedArrayDictionary : public Dictionary<Key, Val> {
private:
    // An element in the dictionary, contains a key and a value
    struct Record {
        Key k;
        Val v;

        Record() : k(Key()), v(Val()) {}
        Record(Key x, Val y) : k(x), v(y) {}
    };

    Record* buffer;
    int maxSize;
    int length;

    void copy(const UnsortedArrayDictionary<Key, Val>&);

public:
    UnsortedArrayDictionary(int maxSize = 100);
    UnsortedArrayDictionary(const UnsortedArrayDictionary<Key, Val>&);
    UnsortedArrayDictionary<Key, Val>& operator=(const UnsortedArrayDictionary<Key, Val>&);
    virtual ~UnsortedArrayDictionary();

    virtual void clear() override;
    virtual Val find(const Key&) const override;
    virtual void insert(const Key&, const Val&) override;
    virtual void remove(const Key&) override;
    virtual int size() const override;
};

// Implementation

template<typename Key, typename Val>
UnsortedArrayDictionary<Key, Val>::UnsortedArrayDictionary(int i)
    : maxSize(i), length(0) {
    buffer = new Record[maxSize];
}

template<typename Key, typename Val>
UnsortedArrayDictionary<Key, Val>::UnsortedArrayDictionary(
    const UnsortedArrayDictionary<Key, Val>& copyObj) {
    copy(copyObj);
}

template<typename Key, typename Val>
UnsortedArrayDictionary<Key, Val>& UnsortedArrayDictionary<Key, Val>::operator=(
    const UnsortedArrayDictionary<Key, Val>& rightObj) {
    if (this != &rightObj) {
        delete[] buffer;
        copy(rightObj);
    }
    return *this;
}

template<typename Key, typename Val>
UnsortedArrayDictionary<Key, Val>::~UnsortedArrayDictionary() {
    delete[] buffer;
}

template<typename Key, typename Val>
void UnsortedArrayDictionary<Key, Val>::clear() {
    length = 0;
}

template<typename Key, typename Val>
void UnsortedArrayDictionary<Key, Val>::copy(
    const UnsortedArrayDictionary<Key, Val>& copyObj) {
    maxSize = copyObj.maxSize;
    length = copyObj.length;
    buffer = new Record[maxSize];
    for (int i = 0; i < length; i++) {
        buffer[i] = copyObj.buffer[i];
    }
}

template<typename Key, typename Val>
Val UnsortedArrayDictionary<Key, Val>::find(const Key& k) const {
    for (int i = 0; i < length; i++) {
        if (buffer[i].k == k) {
            return buffer[i].v;
        }
    }
    throw std::runtime_error("find: error, key not found");
}

template<typename Key, typename Val>
void UnsortedArrayDictionary<Key, Val>::insert(const Key& k, const Val& v) {
    for (int i = 0; i < length; i++) {
        if (buffer[i].k == k) {
            // Key exists - update value
            buffer[i].v = v;
            return;
        }
    }
    if (length >= maxSize) {
        throw std::runtime_error("insert: error, dictionary is full");
    }
    buffer[length] = Record(k, v);
    length++;
}

template<typename Key, typename Val>
void UnsortedArrayDictionary<Key, Val>::remove(const Key& k) {
    for (int i = 0; i < length; i++) {
        if (buffer[i].k == k) {
            buffer[i] = buffer[length - 1];
            length--;
            return;
        }
    }
    throw std::runtime_error("remove: error, key not found");
}

template<typename Key, typename Val>
int UnsortedArrayDictionary<Key, Val>::size() const {
    return length;
}