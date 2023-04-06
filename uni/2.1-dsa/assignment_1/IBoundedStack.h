//
// Created by void on 2/12/22.
//

#ifndef STRUCTS_IBOUNDEDSTACK_H
#define STRUCTS_IBOUNDEDSTACK_H

#include "ICircularBoundedQueue.h"

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

#endif //STRUCTS_IBOUNDEDSTACK_H
