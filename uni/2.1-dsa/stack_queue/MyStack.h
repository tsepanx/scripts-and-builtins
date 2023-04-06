/*
 * MyStack.h
 *
 *  Created on: Jan 22, 2022
 *      Author: firasj
 */

#ifndef MYSTACK_H_
#define MYSTACK_H_
#include "stack.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <optional>


using namespace std;

template<class T> class MyStack: public Stack<T> {

	vector<T> items;

public:

	int size() {
		return items.size();
	}

	bool isEmpty() {
		return size() <= 0;

	}

	optional<T> peek() {
		if (isEmpty()){
//			cout<<"The stack is empty"<<endl;
			return {};
		}

		return items.back();

	}

	optional<T> pop() {
		if (isEmpty()){
//			cout<<"The stack is empty"<<endl;
			return {};
		}
		T item = this->items.back();
		this->items.pop_back();
		return item;
	}

	void push(T value) {
		this->items.push_back(value);
	}

};

#endif /* MYSTACK_H_ */
