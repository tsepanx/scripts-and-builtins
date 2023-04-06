//
// Created by void on 2/12/22.
//

#ifndef LAB02_ILINKEDLIST_H
#define LAB02_ILINKEDLIST_H

using namespace std;


#include <list>
#include <optional>

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

#endif //LAB02_ILINKEDLIST_H