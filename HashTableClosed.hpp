#pragma once

#include "Dictionary.hpp"
#include "hashing.hpp"
#include <stdexcept>
#include <iostream>

template<typename Key, typename Val>
class HashTableClosed : public Dictionary<Key, Val> {
protected:
    // an element in the dictionary, contains a key and a value
    struct Record {
        Key k;
        Val v;

        Record() : k(Key()), v(Val()) {}
        Record(Key x, Val y) : k(x), v(y) {}
    };

    // An enum to denote the state of a slot in the hash table
    enum class SlotType {
        EMPTY, TOMBSTONE, RECORD,
    };

    int M;                 // size of the hash table
    Record* ht;            // array to store records
    SlotType* flags;       // parallel array for slot status
    int probe_constant;    // linear probing constant
    int length;            // number of elements

    // Linear probing function
    int probe(int i) const {
        return probe_constant * i;
    }

public:
    // constructor
    HashTableClosed(int maxSize = 100, int probeSkipNum = 1);

    // destructor
    virtual ~HashTableClosed();

    // dictionary interface methods
    virtual void clear() override;
    virtual Val find(const Key&) const override;
    virtual void insert(const Key&, const Val&) override;
    virtual void remove(const Key&) override;
    virtual int size() const override;

    // for testing purposes
    void print() const;
};

// implementation

template<typename Key, typename Val>
HashTableClosed<Key, Val>::HashTableClosed(int maxSize, int probeSkipNum)
    : M(maxSize), probe_constant(probeSkipNum), length(0) {
    ht = new Record[M];
    flags = new SlotType[M];
    for (int i = 0; i < M; i++) {
        flags[i] = SlotType::EMPTY;
    }
}

template<typename Key, typename Val>
HashTableClosed<Key, Val>::~HashTableClosed() {
    delete[] ht;
    delete[] flags;
}

template<typename Key, typename Val>
void HashTableClosed<Key, Val>::clear() {
    length = 0;
    delete[] ht;
    ht = new Record[M];
    for (int i = 0; i < M; i++) {
        flags[i] = SlotType::EMPTY;
    }
}

template<typename Key, typename Val>
Val HashTableClosed<Key, Val>::find(const Key& k) const {
    int hashValue = cs20::hash(k);
    for (int i = 0; i < M; i++) {
        int index = (hashValue + probe(i)) % M;
        if (index < 0) {
            index += M; // adjust for negative index
        }
        if (flags[index] == SlotType::EMPTY) {
            break;  // key not found
        }
        if (flags[index] == SlotType::RECORD && ht[index].k == k) {
            return ht[index].v;
        }
    }
    throw std::runtime_error("find: error, key not found");
}

template<typename Key, typename Val>
void HashTableClosed<Key, Val>::insert(const Key& k, const Val& v) {
    if (length >= M) {
        throw std::runtime_error("insert: error, the hash table is full");
    }

    int hashValue = cs20::hash(k);
    int first_tombstone = -1;
    for (int i = 0; i < M; i++) {
        int index = (hashValue + probe(i)) % M;
        if (index < 0) {
            index += M; // adjust for negative index
        }
        if (flags[index] == SlotType::EMPTY) {
            if (first_tombstone != -1) {
                index = first_tombstone;
            }
            ht[index] = Record(k, v);
            flags[index] = SlotType::RECORD;
            length++;
            return;
        } else if (flags[index] == SlotType::TOMBSTONE) {
            if (first_tombstone == -1) {
                first_tombstone = index;
            }
        } else if (flags[index] == SlotType::RECORD && ht[index].k == k) {
            // key already exists - update value
            ht[index].v = v;
            return;
        }
    }
    throw std::runtime_error("insert: error, the hash table is full");
}

template<typename Key, typename Val>
void HashTableClosed<Key, Val>::remove(const Key& k) {
    int hashValue = cs20::hash(k);
    for (int i = 0; i < M; i++) {
        int index = (hashValue + probe(i)) % M;
        if (index < 0) {
            index += M; // adjust for negative index
        }
        if (flags[index] == SlotType::EMPTY) {
            break;  // key not found
        }
        if (flags[index] == SlotType::RECORD && ht[index].k == k) {
            flags[index] = SlotType::TOMBSTONE;
            length--;
            return;
        }
    }
    throw std::runtime_error("remove: error, key not found");
}

template<typename Key, typename Val>
int HashTableClosed<Key, Val>::size() const {
    return length;
}

template<typename Key, typename Val>
void HashTableClosed<Key, Val>::print() const {
    for (int i = 0; i < M; i++) {
        std::cout << i << " ";
        switch (flags[i]) {
            case SlotType::EMPTY:
                std::cout << "empty";
                break;
            case SlotType::TOMBSTONE:
                std::cout << "tombstone";
                break;
            case SlotType::RECORD:
                std::cout << "key=" << ht[i].k << ", value=" << ht[i].v;
                break;
        }
        std::cout << std::endl;
    }
}
