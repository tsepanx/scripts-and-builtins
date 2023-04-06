//
// Created by void on 2/12/22.
//

#ifndef STRUCTS_ICIRCULARBOUNDEDQUEUE_H
#define STRUCTS_ICIRCULARBOUNDEDQUEUE_H

#include "LinkedList.h"

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

#endif //STRUCTS_ICIRCULARBOUNDEDQUEUE_H
