#include <iostream>
using namespace std;

typedef struct Node {
    int num;
    Node* next;
} Node;

Node* mySolution(Node* head) {
    /*
        Time complexity: O(n)
        Space complexity: O(1)
    */
    // your code
    Node* front;
    Node* back;
    Node* tmp;
    if (head == NULL) {
        return head;
    }
    if (head->next == NULL) {
        return head;
    }
    // not we can safely assume front and back are both not NULL
    back = head;
    front = back->next;
    head = front;
    while (front->next != NULL && front->next->next != NULL) {
        tmp = front->next;
        front->next = back;
        back->next = tmp->next;
        front = back->next;
        back = tmp;
        // next: ->node->NULL or ->node
    }
    tmp = front->next;
    front->next = back;
    back->next = tmp;
    return head;
}

void test() {
    // 链表 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9

    // Node h10 = {10, NULL};
    // Node h9 = {9, &h10};
    // Node h8 = {8, &h9};
    // Node h7 = {7, &h8};
    // Node h6 = {6, &h7};
    // Node h5 = {5, &h6};
    // Node h4 = {4, &h5};
    // Node h3 = {3, &h4};
    // Node h2 = {2, &h3};
    // Node h1 = {1, &h2};

    Node h1 = {1, NULL};

    // Node h1 = NULL;

    // Node h9 = {9, NULL};
    // Node h8 = {8, &h9};
    // Node h7 = {7, &h8};
    // Node h6 = {6, &h7};
    // Node h5 = {5, &h6};
    // Node h4 = {4, &h5};
    // Node h3 = {3, &h4};
    // Node h2 = {2, &h3};
    // Node h1 = {1, &h2};

    Node* res = mySolution(&h1);
    while (res != NULL) {
        cout << res->num << " ";
        res = res->next;
    }
    cout << endl;
}

int main() {
    test();
    return 0;
}