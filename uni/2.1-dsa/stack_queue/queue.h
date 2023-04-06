/*
 * queue
 *
 *  Created on: Jan 22, 2022
 *      Author: firasj
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <iostream>
#include <optional>

using namespace std;

template <typename T>
class Queue{

public:
	virtual ~Queue(){};
	virtual	void offer(T value) = 0;
	virtual optional<T> poll() = 0;
	virtual optional<T> peek() = 0;
	virtual int size() = 0;
	virtual bool isEmpty() = 0;
};




#endif /* QUEUE_H_ */
