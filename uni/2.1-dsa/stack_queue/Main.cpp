/*
 * Main.cpp
 *
 *  Created on: Jan 22, 2022
 *      Author: firasj
 */

#include <iostream>
#include "MyStack.h"
#include "MyQueue.h"
#include <optional>

using namespace std;

typedef int T; // char

void process_optional(optional<T> item, string msg, string error_msg) {

	if (item.has_value())
		cout << msg << item.value() << endl;
	else
		cout << error_msg << endl;

}

int main() {

	int n = 10;

	MyStack<T> s;
	optional<T> item;

	cout << "STACK" << endl;
	for (int i = 0; i < n; i++) {

		s.push(i);
		cout << "Pushed " << i << endl;

		item = s.peek();
		process_optional(item, "peek ", "Error: cannot peek ");

//		cout<<item.value_or(-1)<<endl;

	}

	for (int i = 0; i < n; i++) {

		item = s.pop();
		process_optional(item, "popped ", "Error: cannot pop");

		item = s.peek();
		process_optional(item, "peek ", "Error: cannot peek ");

	}


	cout<<"=================="<<endl;
	cout << "QUEUE" << endl;
	MyQueue<T> q;

	for (int i = 0; i < n; i++) {

		q.offer(i);
		cout << "Offered " << i << endl;

		item = q.peek();
		process_optional(item, "peek ", "Error: cannot peek ");

	}

	for (int i = 0; i < n; i++) {

		item = q.poll();
		process_optional(item, "polled ", "Error: cannot poll");

		item = q.peek();
		process_optional(item, "peek ", "Error: cannot peek ");

	}

	return 0;

}

