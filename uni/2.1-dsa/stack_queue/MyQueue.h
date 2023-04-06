/*
 * MyQueue.h
 *
 *  Created on: Jan 22, 2022
 *      Author: firasj
 */

#include <iostream>
#include "queue.h"
#include <list>
#include <optional>

using namespace std;

template <class T> class MyQueue : public Queue<T>{

	list<T> items;

public:

	int size(){
		return items.size();
	}

	bool isEmpty(){
		return items.empty();
	}

	void offer(T value){
		items.push_back(value);
	}

	optional<T> poll(){
		if (isEmpty())
			return {};

		T item = items.front();
		items.pop_front();

		return item;
	}

	optional<T> peek(){
		if (isEmpty())
			return {};

		return items.front();
	}

};



