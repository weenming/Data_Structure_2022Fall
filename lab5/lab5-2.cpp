#include <iostream>

#include "myQueue.hpp"
using namespace std;
void push_in_order(Queue<int>*, int);

int* mySolution(int* arr, int length, int k) {
    // 时间复杂度：O(n)
    // 空间复杂度：O(k)
    // 关键是怎么判断因为滑动窗口的出队!
    // 先处理第一个滑动窗口,从右到左升序.(必须要比右边所有都大才能留下)
    // front 在右
    Queue<int> q;
    int* res = new int[length - k + 1];
    for (int i = 0; i < k; i++) {
        push_in_order(&q, arr[i]);
    }
    for (int i = 0; i < length - k; i++) {
        res[i] = q.get_rear();
        if (arr[i] == q.get_rear()) {
            q.pop_rear();
        }
        push_in_order(&q, arr[i + k]);
    }
    res[length - k] = q.get_rear();
    return res;
}

void push_in_order(Queue<int>* q, int data) {
    // 维护一个单调队列,如果即将入队的元素比列尾大,就让列尾出队
    while (1) {
        if ((*q).is_empty()) {
            (*q).push_front(data);
            break;
        }
        // 这里>=保证不会因为元素值相等出问题
        if ((*q).get_front() >= data) {
            (*q).push_front(data);
            break;
        } else {
            (*q).pop_front();
        }
    }
    return;
}

void test() {
    // these test cases can be run with queue maxsize == 4 (cyclic indexing
    // prevented waste)
    //
    // int arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int arr[8] = {1, 2, 3, 3, 3, 6, 7, 8};
    // int arr[8] = {1, 1, 1, 0, 0, 0, 0, 0};
    // int arr[8] = {9, 8, 7, 6, 5, 4, 3, 2};
    // int arr[3] = {1, 2, 3};
    // int arr[1] = {100};
    int* res = mySolution(arr, 8, 4);
    for (int i = 0; i < 6; i++) {
        cout << res[i] << "  ";
    }
    cout << endl;
}

int main() {
    test();
    return 0;
}