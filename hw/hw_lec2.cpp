#include <iostream>
using namespace std;

void max_and_min(int* arr, int len) {
    int max = arr[0];
    int min = arr[0];
    for (int i = 1; i < len; i++) {
        if (arr[i] > max) {
            max = arr[i];
        } else if (arr[i] < min) {
            min = arr[i];
        }
    }
    cout << "max is " << max << endl;
    cout << "min is " << min << endl;
}

int del_elem(int* arr, int len, int elem) {
    for (int i = 0; i < len; i++) {
        if (arr[i] == elem) {
            for (int j = i; j < len - 2; j++) {
                arr[j] = arr[j + 1];
            }
            i--;
            len--;
        }
    }
    return len;
}

void reverse(int* arr, int len) {
    int tmp;
    for (int i = 0; i < len - i; i++) {
        tmp = arr[i];
        arr[i] = arr[len - 1 - i];
        arr[len - i] = tmp;
    }
}

int main(void) {
    // 典型的C++ = C + cin + cout ...
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 200 - i;
    }
    max_and_min(arr, 100);

    int len = 100;
    arr[3] = 105;
    arr[4] = 105;
    len = del_elem(arr, len, 105);
    reverse(arr, len);
    cout << len << endl;
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }

    return 0;
}
