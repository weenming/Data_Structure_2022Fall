#include <iostream>
#include <vector>

#include "MyHeap.hpp"

using namespace std;

int Solution(vector<int> &nums, int k) {
    if (k > nums.size() || k < 1) {
        cout << "invalid k" << endl;
    } else {
        MyHeap<int> heap(nums);
        heap.ShowHeap();
        for (int i = 1; i < nums.size() + 1 - k; i++) {
            // \sum_i^n \log_2 i = \log_2 (\prod_{i=0}^n i) \approx (Stirling's
            // law) n\log_2 n
            heap.PopTop();
            // heap.ShowHeap();
        }
        return heap.PopTop();
    }
    return 0;
}

void test1() {
    // vector<int> nums = {0, 1, 2, 3, 4, 5, 6, 7};
    // cout << Solution(nums, 2) << endl;

    // vector<int> nums = {1, 9, 2, 3, 73, 5, 6, 3};
    // cout << Solution(nums, 1) << endl;

    vector<int> nums = {0, 0, 0, 0, 1, 1, 1, 3, 4, 4};
    cout << Solution(nums, 3) << endl;

    // vector<int> nums = {1};
    // cout << Solution(nums, 1) << endl;
    return;
}

int main() {
    test1();
    return 0;
}