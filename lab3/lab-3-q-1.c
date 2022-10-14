#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} Node;

Node* insertToBeginHasEmptyHead(Node* list, int n) {
    // your answer code
    Node* first = (Node*)malloc(sizeof(Node));
    first->next = list->next;
    first->data = n;
    list->next = first;
    return list;
}

Node* insertToEndHasEmptyHead(Node* list, int n) {
    // your answer code
    Node* last = (Node*)malloc(sizeof(Node));
    Node* node = list;
    while (node->next != NULL) {
        node = node->next;
    }
    node->next = last;
    last->next = NULL;
    last->data = n;
    return list;
}

Node* insertToBeginNotEmptyHead(Node* list, int n) {
    // your answer code
    Node* first = (Node*)malloc(sizeof(Node));
    first->data = n;
    first->next = list;
    list = first;
    return list;
}

Node* insertToEndNotEmptyHead(Node* list, int n) {
    // your answer code
    Node* last = (Node*)malloc(sizeof(Node));
    Node* node = list;
    if (list == NULL) {
        list = (Node*)malloc(sizeof(Node));
        list->next = NULL;
        list->data = n;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = last;
        last->next = NULL;
        last->data = n;
    }
    return list;
}

void destoryListHasEmptyHead(Node* list) {
    // your answer code
    Node* tmp;
    while (list->next != NULL) {
        tmp = list->next;
        list->next = tmp->next;
        free(tmp);
    }
    return;
}

void destoryListNotEmptyHead(Node** listptr) {
    // your answer code
    Node* list = *listptr;
    Node* tmp;
    while (list != NULL) {
        tmp = list;
        list = list->next;
        free(tmp);
    }
    *listptr = NULL;
    return;
}

void printListHasEmptyHead(Node* list);
void printListNotEmptyHead(Node* list);

void testHasEmptyHead() {
    // your test case
    Node* head = (Node*)malloc(sizeof(Node));
    head->data = -1;
    head->next = NULL;
    // test case 0

    // test case 1
    // head = insertToBeginHasEmptyHead(head, 1);
    // head = insertToBeginHasEmptyHead(head, 2);
    // head = insertToBeginHasEmptyHead(head, 3);
    // head = insertToEndHasEmptyHead(head, 10);
    // head = insertToEndHasEmptyHead(head, 20);
    // head = insertToEndHasEmptyHead(head, 30);

    // test case 2
    //  head = insertToBeginHasEmptyHead(head, 100);
    //  head = insertToBeginHasEmptyHead(head, 200);

    // test case 3
    // head = insertToBeginHasEmptyHead(head, 100);

    // test case 4
    // head = insertToEndHasEmptyHead(head, 10);
    // head = insertToEndHasEmptyHead(head, 20);

    // test case 5
    // head = insertToEndHasEmptyHead(head, 20);

    // test case 6
    head = insertToBeginHasEmptyHead(head, 100);
    head = insertToEndHasEmptyHead(head, 20);
    head = insertToBeginHasEmptyHead(head, 2);
    head = insertToBeginHasEmptyHead(head, 3);
    head = insertToEndHasEmptyHead(head, 10);
    head = insertToEndHasEmptyHead(head, 20);

    printListHasEmptyHead(head);
    destoryListHasEmptyHead(head);
    printListHasEmptyHead(head);

    free(head);
    head = NULL;
}

void testNotEmptyHead() {
    // your test case
    Node* head = NULL;
    // test case 0

    // test case 1
    // head = insertToBeginNotEmptyHead(head, 1);
    // head = insertToBeginNotEmptyHead(head, 2);
    // head = insertToBeginNotEmptyHead(head, 3);
    // head = insertToEndNotEmptyHead(head, 10);
    // head = insertToEndNotEmptyHead(head, 20);
    // head = insertToEndNotEmptyHead(head, 30);

    // test case 2
    // head = insertToBeginNotEmptyHead(head, 100);
    // head = insertToBeginNotEmptyHead(head, 200);

    // test case 3
    // head = insertToBeginNotEmptyHead(head, 100);

    // test case 4
    // head = insertToEndNotEmptyHead(head, 10);
    // head = insertToEndNotEmptyHead(head, 20);

    // test case 5
    // head = insertToEndNotEmptyHead(head, 20);

    // test case 6
    head = insertToBeginNotEmptyHead(head, 100);
    head = insertToEndNotEmptyHead(head, 20);
    head = insertToBeginNotEmptyHead(head, 2);
    head = insertToBeginNotEmptyHead(head, 3);
    head = insertToEndNotEmptyHead(head, 10);
    head = insertToEndNotEmptyHead(head, 20);

    printListNotEmptyHead(head);
    // have not figured out how to set the input pointer to NULL...
    destoryListNotEmptyHead(&head);
    printListNotEmptyHead(head);
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