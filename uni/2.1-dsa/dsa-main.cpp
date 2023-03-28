// 
// Created by Tsepa Stepan (s.tsepa@innopolis.university)
//

#include <iostream>
#include <list>
#include <optional>
#include <vector>
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

void print_2_arrs(const vector<string>& v1, const vector<string>& v2) {
    for (string i : v1) {
        cout << i << ' ';
    }

    for (string i : v2) {
        cout << i << ' ';
    }
    cout << '\n';
}

template <class T> struct Node {
    T elem;
    Node* next;
};

template <class T> class LinkedList {
private:
    Node<T>* front;
    Node<T>* back;
public:
    LinkedList() {
        this->front = nullptr;
        this->back = nullptr;
    }

    int length() {
        if (this->front == nullptr) { return 0; }

        int count = 1;
        auto* cur = this->front;

        while (cur->next != nullptr) {
            cur = cur->next;
            count += 1;
        }

        return count;
    }

    bool isEmpty() {
        return (this->length() == 0);
    }

    // Time complexity: O(1)
    void append_back(T new_elem) {
        auto* tmp = new Node<T>;
        tmp->elem = new_elem;
        tmp->next = nullptr;

        if (front == nullptr) {
            front = tmp;
            back = tmp;
        } else {
            back->next = tmp;
            back = tmp;
        }
    }

    // Time complexity: O(1)
    void append_front(T new_elem) {
        auto* tmp = new Node<T>;
        tmp->elem = new_elem;
        tmp->next = nullptr;

        if (front == nullptr) {
            front = tmp;
            back = tmp;
        } else {
            tmp->next = front;
            front = tmp;
        }
    }

    // Time complexity: O(1)
    optional<T> pop_front() {
        if (this->isEmpty()) { return {}; }

        if (this->length() == 1) {
            auto res = front;
            front = nullptr;
            back = nullptr;
            return res->elem;
        } else {
            auto old_front = front;
            front = front->next;
            T result_elem = old_front->elem;
            delete old_front;
            return result_elem;
        }
    }

    // Time complexity: O(1)
    optional<T> get_front() {
        if (this->isEmpty()) { return {}; }

        return (front->elem);
    }

    // Time complexity: O(n)
    void flush() {
        while (this->pop_front().has_value()) {}
    }
};

template <class T> class ICircularBoundedQueue {
public:
    virtual void offer(T value) = 0; // insert an element to the rear of the queue
    // overwrite the oldest elements
    // when the queue is full
    virtual optional<T> poll() = 0; // remove an element from the front of the queue
    virtual optional<T> peek() = 0; // look at the element at the front of the queue
    // (without removing it)
    virtual void flush() = 0;
    // remove all elements from the queue
    virtual bool isEmpty() = 0; // is the queue empty?
    virtual bool isFull() = 0;
    // is the queue full?
    virtual int size() = 0;
    // number of elements
    virtual int capacity() = 0; // maximum capacity
};

template <class T> class LinkedCircularBoundedQueue: ICircularBoundedQueue<T> {
private:
    LinkedList<T> items;
    int max_length;

public:
    LinkedCircularBoundedQueue(int capacity) {
        this->max_length = capacity;
    }

    // Time complexity: O(1)
    void offer(T value) {
        if (this->isFull()) {
            items.pop_front();
        }

        items.append_back(value);
    }

    // Time complexity: O(1)
    optional<T> poll() {
        if (this->isEmpty()) {
            return {};
        }

        return items.pop_front();
    }

    // Time complexity: O(1)
    optional<T> peek() {
        return items.get_front();
    }

    // Time complexity: O(n)
    void flush() {
        items.flush();
    }

    // Time complexity: O(1)
    bool isEmpty() {
        return (items.length() == 0);
    }

    // Time complexity: O(1)
    bool isFull() {
        return (items.length() == this->max_length);
    }

    // Time complexity: O(1)
    int size() {
        return items.length();
    }

    // Time complexity: O(1)
    int capacity() {
        return this->max_length;
    }
};

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
    optional<T> items[1000000];
    int capacity;
    int current_elements_count;

    int hash1(T value) {
        return hash<T>()(value) % capacity;
    }

    int my_hashcode(string value) {
        int sum = 0;

        for (char i: value) {
            sum = sum * 2 + (int)i;
        }
        return sum % capacity;
    }

    int hash2(T value) {
        int c;
        c = this->capacity - 1;
        while (!is_prime(c)) {
            c--;
        }

        return c - my_hashcode(value) % c;
    }

public:

    DoubleHashSet(int size) {
        this->capacity = size;
        this->current_elements_count = 0;
    }

    // Time complexity: Best - O(1); Worst - O(n)
    void add(T key) {
        if (this->size() >= capacity) { throw false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur;
        for (int i = 0; i < this->capacity; ++i) {
            cur = (h1 + i * h2) % capacity;

            if (!items[cur].has_value()) {
                items[cur] = key;
                current_elements_count++;
                return;
            }

            if (items[cur].value() == key) {
                throw false;
            }
        }
    };

    // Time complexity: Best - O(1); Worst - O(n)
    void remove(T key) {
        if (this->size() == 0) { throw false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur;
        for (int i = 0; i < this->capacity; ++i) {
            cur = (h1 + i * h2) % capacity;

            optional<T> cur_elem = items[cur];

            if (cur_elem.has_value()) {
                if (cur_elem.value() == key) {
                    items[cur] = ""; // Setting cur item as empty string instead of null, to mark this element as deleted
                    current_elements_count--;
                    return;
                }
            } else {
                throw false;
            }
        }
    };

    // Time complexity: Best - O(1); Worst - O(n)
    bool contains(T key) {
        if (this->size() == 0) { return false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur;
        for (int i = 0; i < this->capacity; ++i) {
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

    // Time complexity: O(1)
    int size() {
        return this->current_elements_count;
    };

    // Time complexity: O(1)
    bool isEmpty() {
        return (this->size() == 0);
    };

    vector<T> get_values() {
        auto res = vector<T>();
        int items_size = (sizeof(items)/sizeof(*items));
        for (int i = 0; i < items_size; ++i) {
            if (items[i].has_value()) {
                res.push_back(items[i].value());
            }
        }
        return res;
    }
};

int main() {
    int n; cin >> n;
    int k = 1000000;
    auto s_file = DoubleHashSet<string>(k);
    auto s_dir = DoubleHashSet<string>(k);

    for (int i = 0; i <= n; ++i) {
        string command, path;
        cin >> command;
        if (command == "LIST") {
            vector<string> dirs = s_dir.get_values();
            vector<string> files = s_file.get_values();

            print_2_arrs(dirs, files);
        } else { cin >> path; }

        try {
            if (command == "NEW") {
                if (path.back() == '/') {
                    if (!s_file.contains(path.substr(0, path.size()-1))) {
                        s_dir.add(path);
                    } else { throw false; }
                } else {
                    if (!s_dir.contains(path + '/')) {
                        s_file.add(path);
                    } else { throw false; }
                }
            } else if (command == "REMOVE") {
                if (path.back() == '/') {
                    s_dir.remove(path);
                } else {
                    s_file.remove(path);
                }
            }
        } catch (...) {
            cout << "ERROR: cannot execute " << command << " " << path << '\n';
        }
    }
}
