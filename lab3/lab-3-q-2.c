#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} Node;

Node* reverseListHasEmptyHead(Node* list) {
    // your answer code
    Node* work = list->next;
    Node* workNext;
    while (work != NULL && work->next != NULL) {
        // link the end of revered part to the beginning non-reversed part
        workNext = work->next;
        work->next = work->next->next;
        // insert next to head
        workNext->next = list->next;
        list->next = workNext;
    }
    return list;
}

Node* reverseListNotEmptyHead(Node* list) {
    // your answer code
    Node* work = list;
    Node* workNext;
    while (work != NULL && work->next != NULL) {
        // link the end of revered part to the beginning non-reversed part
        workNext = work->next;
        work->next = work->next->next;
        // insert in the beginning
        workNext->next = list;
        list = workNext;
    }
    return list;
}

void printListHasEmptyHead(Node* list);
void printListNotEmptyHead(Node* list);

void testHasEmptyHead() {
    // test case 1
    // Node h;
    // h.data = -1;
    // Node a;
    // a.data = 1;
    // Node b;
    // b.data = 2;
    // Node c;
    // c.data = 3;
    // h.next = &a;
    // a.next = &b;
    // b.next = &c;
    // c.next = NULL;
    // // h.next = NULL;
    //
    // test case 2
    // Node h;
    // h.data = -1;
    // Node a;
    // a.data = 1;
    // Node b;
    // b.data = 2;
    // h.next = &a;
    // a.next = &b;
    // b.next = NULL;

    // test case 3
    // Node h;
    // h.data = -1;
    // Node a;
    // a.data = 1;
    // h.next = &a;
    // a.next = NULL;

    // test case 4
    // Node h;
    // h.data = -1;
    // h.next = NULL;

    // test case 5
    Node h;
    h.data = -1;
    Node a;
    a.data = 1;
    Node b;
    b.data = 2;
    Node c;
    c.data = 3;
    Node d;
    d.data = 4;
    Node e;
    e.data = 5;
    h.next = &a;
    a.next = &b;
    b.next = &c;
    c.next = &d;
    d.next = &e;
    e.next = NULL;

    Node* list = &h;

    printListHasEmptyHead(list);
    list = reverseListHasEmptyHead(list);
    printListHasEmptyHead(list);
}
void testNotEmptyHead() {
    // test case 1
    Node a;
    a.data = 1;
    Node b;
    b.data = 2;
    Node c;
    c.data = 3;
    a.next = &b;
    b.next = &c;
    c.next = NULL;

    //
    // test case 2
    // Node a;
    // a.data = 1;
    // Node b;
    // b.data = 2;
    // a.next = &b;
    // b.next = NULL;

    // test case 3
    // Node a;
    // a.data = 1;
    // a.next = NULL;

    // test case 4
    // uncomment line 156 in this test case

    // test case 5
    // Node a;
    // a.data = 1;
    // Node b;
    // b.data = 2;
    // Node c;
    // c.data = 3;
    // Node d;
    // d.data = 4;
    // Node e;
    // e.data = 5;
    // a.next = &b;
    // b.next = &c;
    // c.next = &d;
    // d.next = &e;
    // e.next = NULL;

    Node* list = &a;
    // in test case 4, uncomment the line below
    // list = NULL;
    printListNotEmptyHead(list);
    list = reverseListNotEmptyHead(list);
    printListNotEmptyHead(list);
}

int main(void) {
    testHasEmptyHead();
    testNotEmptyHead();
    return 0;
}

void printListHasEmptyHead(Node* list) {
    while (list->next != NULL) {
        printf("%d ", list->next->data);
        list = list->next;
    }
    printf("\n");
    return;
}

void printListNotEmptyHead(Node* list) {
    while (list != NULL) {
        printf("%d ", list->data);
        list = list->next;
    }
    printf("\n");
    return;
}