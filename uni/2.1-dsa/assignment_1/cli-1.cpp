//
// Created by void on 2/13/22.
//

#include "ICircularBoundedQueue.h"
#include <iostream>

using namespace std;

int main() {
    int n, k; cin >> n >> k;
    auto q = LinkedCircularBoundedQueue<string>(k);


//    cin.ignore(' ');
    for (int i = 0; i < n; ++i) {
        string s;
        getline(cin, s);
        q.offer(s);
    }

    int q_size = q.size();

    for (int i = 0; i < q_size; ++i) {
        cout << q.poll().value() << '\n';
    }
}