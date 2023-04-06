
#include <iostream>
#include <list>
#include <optional>
#include <vector>
#include <math.h>

using namespace std;

typedef int T;

bool is_prime(int n) {
    int m = (int) sqrt(n);
    for (int i = 2; i <= m; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
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

    optional<T> get_front() {
        if (this->isEmpty()) { return {}; }

        return (front->elem);
    }

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

    void offer(T value) {
        if (this->isFull()) {
            items.pop_front();
        }

        items.append_back(value);
    }

    optional<T> poll() {
        if (this->isEmpty()) {
            return {};
        }

        return items.pop_front();
    }

    optional<T> peek() {
        return items.get_front();
    }

    void flush() {
        items.flush();
    }

    bool isEmpty() {
        return (items.length() == 0);
    }

    bool isFull() {
        return (items.length() == this->max_length);
    }

    int size() {
        return items.length();
    }

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

    int hash2(T value) {
        int c;
        c = this->capacity - 1;
        while (!is_prime(c)) {
            c--;
        }

        return c - hash1(value) % c;
    }

public:

    DoubleHashSet(int size) {
        this->capacity = size;
//        this->items = vector<optional<T>>(capacity, nullopt);
        this->current_elements_count = 0;
    }

    void add(T key) {
        if (this->size() >= capacity) { throw false; }

        int h1 = hash1(key);
        int h2 = hash2(key);

        int cur ;
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
//                    items[cur] = nullopt;
                    items[cur] = "";
                    current_elements_count--;
                    return;
                }
            } else {
                throw false;
            }
        }

//        if (!items[cur].has_value()) { throw false; }
    };

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

    int size() {
        return this->current_elements_count;
    };

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

typedef int T;

void swap(T *r, T *s) {
    int temp = *r;
    *r = *s;
    *s = temp;
}

template <class T> class IBoundedStack {
    virtual void push(T value) = 0; // push an element onto the stack
    // remove the oldest element
    // when if stack is full
    virtual optional<T> pop() = 0; // remove an element from the top of the stack
    virtual optional<T> top() = 0; // look at the element at the top of the stack
    // (without removing it)
    virtual void flush() = 0;
    // remove all elements from the stack
    virtual bool isEmpty() = 0; // is the stack empty?
    virtual bool isFull() = 0;
    // is the stack full?
    virtual int size() = 0;
    // number of elements
    virtual int capacity() = 0; // maximum capacity
};

template <class T> class QueuedBoundedStack: IBoundedStack<T> {
private:
    LinkedCircularBoundedQueue<T>* q_main;
    LinkedCircularBoundedQueue<T>* q_support;
    int max_length;

public:
    QueuedBoundedStack(int capacity) {
        q_main = new LinkedCircularBoundedQueue<T>(capacity);
        q_support = new LinkedCircularBoundedQueue<T>(capacity);
        this->max_length = capacity;
    }

    void push(T value) {
        q_support->offer(value);
        while ((!q_main->isEmpty()) and (!q_support->isFull())) {
            q_support->offer(q_main->poll().value());
        }
        swap(q_main, q_support);
    }

    optional<T> pop() {
        return q_main->poll();
    }

    optional<T> top() {
        return q_main->peek();
    }

    void flush() {
        q_main->flush();
    }

    bool isEmpty() {
        return q_main->isEmpty();
    }

    bool isFull() {
        return q_main->isFull();
    }

    int size() {
        return q_main->size();
    }

    int capacity() {
        return this->max_length;
    }

};

void print_2_arrs(const vector<string>& v1, const vector<string>& v2) {
    for (string i : v1) {
        cout << i << ' ';
    }

    for (string i : v2) {
        cout << i << ' ';
    }
    cout << '\n';
}

int main() {
    int n; cin >> n;
    int k = 1000000;
//    int k = n;
    auto q = QueuedBoundedStack<DoubleHashSet<string>>(n);

    auto s_file = DoubleHashSet<string>(k);
    auto s_dir = DoubleHashSet<string>(k);

    for (int i = 0; i <= n; ++i) {

        char line[100];

        cin.getline (line, 100);
        string s_line = string(line);
        if (s_line == "LIST") {
            vector<string> dirs = s_dir.get_values();
            vector<string> files = s_file.get_values();

            print_2_arrs(dirs, files);
        } else if (s_line[0] == 'U') {
            string s_undo_n = s_line.substr(5, s_line.length()-1);
            int undo_n = stoi(s_undo_n);
        } else {
            int sp_i = s_line.find(' ');
            string command = s_line.substr(0, sp_i);
            string path = s_line.substr(sp_i + 1, s_line.length() - 1);
//            cin >> command >> path;

            try {
                if (command == "NEW") {
                    if (path.back() == '/') {
                        if (!s_file.contains(path.substr(0, path.size() - 1))) {
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
                } else if (command == "UNDO") {

                }
            } catch (...) {
                cout << "ERROR: cannot execute " << command << " " << path << '\n';
            }
        }
    }
}