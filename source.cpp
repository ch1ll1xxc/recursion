#include <iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
using namespace std;
int ch = 0;

int NOD(int a, int b) {
    ch++;
    if (b == 0) {
        return a;
    }
    else {
        return NOD(b, a % b);
    }
}

int main() {
    while (true) {
        int a, b;
        cout << "Enter two integer numbers (-1 -1 is exit): ";
        cin >> a >> b;
        if (a == -1 && b == -1) { cout << "times: " << ch; return 0; }
        unsigned int start_time = clock();
        int result = NOD(a, b);
        unsigned int end_time = clock();
        unsigned int time = end_time - start_time;
        //cout << "NOD(" << a << ", " << b << ") = " << result << endl;
    }
}

//struct Node {
//    int data;
//    Node* next;
//};
//
//Node* create(int n) {
//    Node* head = nullptr;
//    Node* tail = nullptr;
//
//    for (int i = 1; i <= n; i++) {
//        Node* newNode = new Node;
//        //cout << "Enter node`s info: " << i << ": ";
//        int tmp = rand();
//        newNode->data = tmp;
//
//        newNode->next = nullptr;
//        if (head == nullptr) {
//            head = newNode;
//            tail = newNode;
//        }
//        else {
//            tail->next = newNode;
//            tail = newNode;
//        }
//    }
//
//    return head;
//}
//
//void print(Node* head) {
//    Node* current = head;
//    while (current != nullptr) {
//        cout << current->data << " ";
//        current = current->next;
//    }
//    cout << endl;
//}
//
//
//int main() {
//    vector <pair<int, unsigned int>> data;
//    while (true) {
//
//        int n;
//        cout << "Enter amount of nodes (-1 is exit): ";
//        cin >> n;
//        if (n == -1) { for (auto a : data) { cout << "Amount of nodes: " << a.first << " " << "Time: " << a.second << endl; }; return 0; }
//        
//        Node* head = create(n);
//        unsigned int start_time = clock();
//        cout << "List: ";
//        print(head);
//        unsigned int end_time = clock();
//        unsigned int time = end_time - start_time;
//        //cout << "Time: " << end_time - start_time << endl;
//        data.push_back({ n, time });
//    }
//    
//
//}