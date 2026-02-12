#pragma once

#include "Dictionary.hpp"
#include "hashing.hpp"
#include <stdexcept>
#include <iostream>

template<typename Key, typename Val>
class HashTableOpened : public Dictionary<Key, Val> {
protected:
    // an element in the dictionary, contain a key and a value
    struct Record {
        Key k;
        Val v;

        Record() : k(Key()), v(Val()) {}
        Record(const Key& x, const Val& y) : k(x), v(y) {}
    };

    // node for the linked list in each bucket
    struct Node {
        Record data;
        Node* next;

        Node(const Record& r, Node* n = nullptr) : data(r), next(n) {}
    };

    int M;         // size of hash table (number of buckets)
    Node** table;  // array of pointers to linked lists (buckets)
    // the double pointer Node** table is for makin an array of pointers, and each pointer Node*
    // is the start of a linked list (bucket) in the hash table. it's used to handle collisions by chaining.
    int length;    // number of elements in hash table

public:
    // constructor
    HashTableOpened(int maxSize = 100);

    // destructor
    virtual ~HashTableOpened();

    // dictionary interface methods
    virtual void clear() override;
    virtual Val find(const Key&) const override;
    virtual void insert(const Key&, const Val&) override;
    virtual void remove(const Key&) override;
    virtual int size() const override;

    // for testing purpose
    void print() const;
};

// implementation

template<typename Key, typename Val>
HashTableOpened<Key, Val>::HashTableOpened(int maxSize)
    : M(maxSize), length(0) {
    // initialize table with null pointer
    table = new Node*[M];
    for (int i = 0; i < M; ++i) {
        table[i] = nullptr;
    }
}

template<typename Key, typename Val>
HashTableOpened<Key, Val>::~HashTableOpened() {
    clear();
    delete[] table;
}

template<typename Key, typename Val>
void HashTableOpened<Key, Val>::clear() {
    // iterate over each bucket and delete the linked lists
    for (int i = 0; i < M; ++i) {
        Node* current = table[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        table[i] = nullptr;
    }
    length = 0;
}

template<typename Key, typename Val>
Val HashTableOpened<Key, Val>::find(const Key& k) const {
    int hashValue = cs20::hash(k) % M;
    if (hashValue < 0) {
        hashValue += M; // adjust for negative hash values
    }
    Node* current = table[hashValue];
    while (current != nullptr) {
        if (current->data.k == k) {
            return current->data.v;
        }
        current = current->next;
    }
    throw std::runtime_error("find: error, key not found");
}

template<typename Key, typename Val>
void HashTableOpened<Key, Val>::insert(const Key& k, const Val& v) {
    int hashValue = cs20::hash(k) % M;
    if (hashValue < 0) {
        hashValue += M; // adjust for negative hash values
    }
    Node* current = table[hashValue];
    while (current != nullptr) {
        if (current->data.k == k) {
            // key exists - update value
            current->data.v = v;
            return;
        }
        current = current->next;
    }
    // key not found - insert new record at the begining
    Node* newNode = new Node(Record(k, v), table[hashValue]);
    table[hashValue] = newNode;
    length++;
}

template<typename Key, typename Val>
void HashTableOpened<Key, Val>::remove(const Key& k) {
    int hashValue = cs20::hash(k) % M;
    if (hashValue < 0) {
        hashValue += M; // adjust for negative hash values
    }
    Node* current = table[hashValue];
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->data.k == k) {
            // key found - remove node
            if (prev == nullptr) {
                // node is at the head of the list
                table[hashValue] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            length--;
            return;
        }
        prev = current;
        current = current->next;
    }
    throw std::runtime_error("remove: error, key not found");
}

template<typename Key, typename Val>
int HashTableOpened<Key, Val>::size() const {
    return length;
}

template<typename Key, typename Val>
void HashTableOpened<Key, Val>::print() const {
    for (int i = 0; i < M; ++i) {
        std::cout << "bucket " << i << ": ";
        Node* current = table[i];
        while (current != nullptr) {
            std::cout << "[" << current->data.k << ": " << current->data.v << "] ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}
