//
// Created by void on 2/12/22.
//

#ifndef STRUCTS_ISET_H
#define STRUCTS_ISET_H

#include <vector>
#include <optional>
#include <math.h>

using namespace std;

bool is_prime(int n) {
    int m = (int) sqrt(n);
    for (int i = 2; i <= m; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

template <class T> class ISet {
    virtual void add(T item) = 0;
                // add item in the set
    virtual void remove(T item) = 0;
                // remove an item from a set
    virtual bool contains(T item) = 0; // check if a item belongs to a set
    virtual int size() = 0;
                // number of elements in a set
    virtual bool isEmpty() = 0;
                // check if the set is empty
};

template <class T> class DoubleHashSet: ISet<T> {
private:
    vector<optional<T>> items;
    int capacity;
    int current_elements_count;

    int hash1(T value) {
//        auto hashfunc = hash<T>();
//        int hashcode = hashfunc(value);

        return hash<T>()(value) % capacity;
    }

    int hash2(string value) {
        int sum = 0;

        for (char i: value) {
            sum = sum * 2 + (int)i;
        }
        return sum % capacity;
    }

public:

    DoubleHashSet(int size) {
        this->capacity = size;
        this->items = vector<optional<T>>(capacity, nullopt);
        this->current_elements_count = 0;
    }

    void add(T key) {
        if (this->size() >= capacity) { throw false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur ;
        for (int i = 0; i < this->size() + 1; ++i) {
            cur = (h1 + i * h2) % capacity;

            if (!items[cur].has_value()) {
                items[cur] = key;
                current_elements_count++;
                return;
            } else if (items[cur].value() == key) { throw false; }
        }
    };

    void remove(T key) {
        if (this->size() == 0) { throw false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur;
        for (int i = 0; i < this->size(); ++i) {
            cur = (h1 + i * h2) % capacity;

            optional<T> cur_elem = items[cur];

            if (cur_elem.has_value()) {
                if (cur_elem.value() == key) {
                    items[cur] = nullopt;
                    current_elements_count--;
                    return;
                }
            } else {
                throw false;
            }
        }

        if (!items[cur].has_value()) { throw false; }
    };

    bool contains(T key) {
        if (this->size() == 0) { return false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur;
        for (int i = 0; i < this->size(); ++i) {
            cur = (h1 + i * h2) % capacity;

            optional<T> cur_elem = items[cur];

            if (cur_elem.has_value()) {
                if (cur_elem.value() == key) {
                    return true;
                }
            } else {
                return false;
            }
        }
        return false;
    };

    int size() {
        return this->current_elements_count;
    };

    bool isEmpty() {
        return (this->size() == 0);
    };

    vector<T> get_values() {
        auto res = vector<T>();
        for (int i = 0; i < this->items.size(); ++i) {
            if (items[i].has_value()) {
                res.push_back(items[i].value());
            }
        }
        return res;
    }
};

#endif //STRUCTS_ISET_H
