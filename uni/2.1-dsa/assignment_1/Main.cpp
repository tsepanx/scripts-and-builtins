#include <bits/stdc++.h>
#include <sstream>
#include <string>
using namespace std;

template <class T> struct Node {
    T elem;
    Node* next;
};


template <class T> class LinkedList {
private:
    Node<T>* head;

    Node<T>* swap_next(Node<int>* cur) {
        if (cur == nullptr || cur->next == nullptr)
            return cur;

        // cur->second->third->...
        auto second = cur->next;
        auto third = second->next;

        second->next = cur;

        // Recursively set next of cur to calculated most left node
        cur->next = this->swap_next(third);

        // second became left, and cur became right: second->cur->swap_next(third)
        return second;
    }
public:
    LinkedList() {
        this->head = nullptr;
    }

    void push(T item) {
        auto* tmp = new Node<T>;

        tmp->elem = item;
        tmp->next = this->head;

        this->head = tmp;
    }

    void swap_all() {
        this->head = this->swap_next(this->head);
    }

    void print() {
        auto* cur = this->head;
        while (cur != nullptr) {
            cout << cur->elem << " ";
            cur = cur->next;
        }
        cout << '\n';
    }
};

vector<string> split(string s) {
    char delim = ' ';
    auto result = vector<string>();

    s = s + delim;

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + 1);
    }

    return result;
}


int main() {
    auto l = LinkedList<int>();

//    char string[10000]; // head = 1 2 3 4

//    cin.getline(string, 10000);

//    std::ifstream infile("/home/void/DSA/DSA1/Structs/DSA_Assignment1/input.txt");
//    freopen("/home/void/DSA/DSA1/Structs/DSA_Assignment1/output.txt", "w", stdout);

    std::ifstream infile("input.txt");
    freopen("output.txt", "w", stdout);

    std::string line;
    std::getline(infile, line);

    auto v = split(line);
    std::reverse(v.begin(), v.end());

    for (int i = 0; i < v.size(); ++i) {
//        cout << v[i] << '\n';
        int n = stoi(v[i]);

        l.push(n);
    }

//    l.print();
    l.swap_all();
    l.print();

    return 0;
}