#include <stdio.h>

int rmDupFromSortArr(int *arr, int len) {
    // time complexity: O(n)
    // space complexity: O(n)

    // find length of new arr
    int new_len = 1;
    for (int i = 0; i < len - 1; ++i) {
        if (arr[i] != arr[i + 1]) {
            new_len += 1;
        }
    }

    // store the none repetitive arr into new_arr
    int new_arr[new_len];
    int j = 1;
    new_arr[0] = arr[0];
    for (int i = 0; i < len - 1; ++i) {
        if (arr[i] != arr[i + 1]) {
            new_arr[j] = arr[i + 1];
            j++;
        }
    }

    // copy new_arr to arr
    for (int i = 0; i < new_len; i++) {
        arr[i] = new_arr[i];
    }
    return new_len;
}

int rmDupFromSortArrDoublePtr(int *arr, int len) {
    // time complexity: O(n)
    // space complexity: O(1)
    int slow = 0;
    int fast = 1;
    for (; fast < len;) {
        if (arr[slow] == arr[fast]) {
            // two ptr same, meaning they are in the same repetitive region
            fast++;
        } else {
            // elements are not identical. copy.
            // if the points are 1 index from each other, no effect
            // if more than 1, move next different element to index slow+1
            // therefore, arr[slow] and arr[fast] always have neiboring elements
            arr[slow + 1] = arr[fast];
            slow++;
            fast++;
        }
    }
    // index -> length: +1
    return slow + 1;
}

void test1() {
    const int len = 10;
    // int arr[9] = {1, 3, 5, 5, 5, 7, 9, 9, 11};
    // int arr[1] = {1};
    // int arr[4] = {2, 2, 2, 2};
    int arr[10] = {2, 2, 2, 3, 4, 5, 5, 8, 8, 8};
    int newLen = rmDupFromSortArrDoublePtr(arr, len);

    for (int i = 0; i < newLen; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return;
}

int main() {
    test1();
    return 0;
}