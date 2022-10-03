#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
} Node;

Node* myMerge1(Node* list1, Node* list2) {
    /* When declaring a pointer, memory needs to be allocated.
    Some say that the attributes need initializing too, but the 
    code here seems to work fine */ 
    Node* merged_list = (Node*)malloc(sizeof(Node));

    // I believe using this block of code below instead of creating a new node should
    // save some memory and time. But I'd rather keep the code neat.

    // make the head of the merged list
    // pick the smaller head between list1 and 2
    // if (list1->data > list2->data){
    //     // assign address of node with smaller data to merged_list (output list)
    //     merged_list = list2;
    //     list2 = list2->next;   
    // }
    // else{
    //     merged_list = list1;
    //     list1 = list1->next;
    // }

    // store head node of the merged list
    Node* head_merged = merged_list;
    
    // make the merged list
    while (1){
        // If neither of the lists has ended, connect the output list to 
        // the input list with smaller data
        if (list1 != NULL && list2 != NULL){
            if (list1->data > list2->data){
                // attach smaller input list to ouput list
                merged_list->next = list2;
                // this node in list2 has been merged, move list2 to next node
                list2 = list2->next;
                // move output list to the end so that attchment is easy
                merged_list = merged_list->next;
                /* The above 2 lines can be swapped. As long 
                as the attributes of a node (data or next) remain unchanged, 
                list2 and merged_list won't be changed simultaneously */
            }
            else{
                merged_list->next = list1;
                list1 = list1->next;
                merged_list = merged_list->next;
            }
        }
        // If one of the input lists has ended, connect the remaining
        // part of the other input list to the output list
        else if (list1 == NULL && list2 != NULL){
            merged_list->next = list2;
            merged_list = merged_list->next;
            break;
        }
        else if (list1 != NULL && list2 == NULL)
        {
            merged_list->next = list1;
            merged_list = merged_list->next;
            break;
        }
        else{
            break;
        }
    }
    return head_merged->next;
}


void printList(Node* list) {
    Node* head = list;
    while (head != NULL) {
        printf("%d ", head->data);
        head = head->next;
    }
    printf("\n");
}

void test() {
    // 这里写测试用例，如
    // test 1
    Node a1 = {10, NULL};
    Node a2 = {30, NULL};
    Node a3 = {50, NULL};
    a1.next = &a2;
    a2.next = &a3;
    Node* lista = &a1;
    Node b1 = {20, NULL};
    Node b2 = {40, NULL};
    Node b3 = {60, NULL};
    b1.next = &b2;
    b2.next = &b3;
    Node* listb = &b1;
    Node* list = myMerge1(lista, listb);
    printList(list);

    // // test 2
    // Node a1 = {20, NULL};
    // Node* lista = &a1;
    // Node b1 = {10, NULL};
    // Node* listb = &b1;
    // Node* list = myMerge1(lista, listb);
    // printList(list);

    // // test 3
    // Node a1 = {10, NULL};
    // Node* lista = &a1;
    // Node b1 = {10, NULL};
    // Node* listb = &b1;
    // Node* list = myMerge1(lista, listb);
    // printList(list);

    //  // test 4
    // Node a1 = {10, NULL};
    // Node a2 = {20, NULL};
    // Node a3 = {30, NULL};
    // a1.next = &a2;
    // a2.next = &a3;
    // Node* lista = &a1;
    // Node b1 = {10, NULL};
    // Node* listb = &b1;
    // Node* list = myMerge1(lista, listb);
    // printList(list);

    // // test 5
    // Node a1 = {10, NULL};
    // Node a2 = {30, NULL};
    // Node a3 = {50, NULL};
    // a1.next = &a2;
    // a2.next = &a3;
    // Node* lista = &a1;
    // Node b1 = {20, NULL};
    // Node b2 = {70, NULL};
    // Node b3 = {70, NULL};
    // b1.next = &b2;
    // b2.next = &b3;
    // Node* listb = &b1;
    // Node* list = myMerge1(lista, listb);
    // printList(list);

    // // test 5
    // Node a1 = {10, NULL};
    // Node a2 = {30, NULL};
    // Node a3 = {50, NULL};
    // a1.next = &a2;
    // a2.next = &a3;
    // Node* lista = &a1;
    // Node b1 = {70, NULL};
    // Node b2 = {70, NULL};
    // Node b3 = {70, NULL};
    // b1.next = &b2;
    // b2.next = &b3;
    // Node* listb = &b1;
    // Node* list = myMerge1(lista, listb);
    // printList(list);


    system("pause");
}

int main(void) {
    test();
    return 0;
}