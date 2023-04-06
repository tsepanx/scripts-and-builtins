/*
 * stack.h
 *
 *  Created on: Jan 22, 2022
 *      Author: firasj
 */

#ifndef STACK_H_
#define STACK_H_

#include <iostream>
#include <optional>

using namespace std;

template <class T> class Stack{

public:
	virtual ~Stack(){};
	virtual void push(T value) = 0;
	virtual optional<T> pop() = 0;
	virtual optional<T> peek() = 0;
	virtual int size() = 0;
	virtual bool isEmpty() = 0;

};



#endif /* STACK_H_ */
