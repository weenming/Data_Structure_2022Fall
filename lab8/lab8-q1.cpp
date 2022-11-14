#include <iostream>
#include <vector>

#include "MyHeap.hpp"

using namespace std;

int SolFast(vector<int> &nums, int k) {
    // has bug...
    int pivot = 0, start = 1, len = nums.size(), end = nums.size(), tmp;
    while (1) {
        for (int i = start; i < end; i++) {
            if (nums[pivot] > nums[i]) {
                // swap
                tmp = nums[start];
                nums[start] = nums[i];
                nums[i] = tmp;
                start++;
            }
        }
        // x x x pivot x x x start - 1 x x x end x x x
        //
        if (start - 1 == len - k) {
            break;
        } else if (start - 1 > len - k) {
            // swap pivot with start - 1
            // then pivot is really what it is
            tmp = nums[pivot];
            nums[pivot] = nums[start - 1];
            nums[start - 1] = tmp;

            end = start - 1;
            start = pivot + 1;
        } else {
            tmp = nums[pivot];
            nums[pivot] = nums[start - 1];
            nums[start - 1] = tmp;

            pivot = start;
            start++;
        }
    }
    return nums[pivot];
}
int Solution(vector<int> &nums, int k) {
    if (k > nums.size() || k < 1) {
        cout << "invalid k" << endl;
    } else {
        MyHeap<int> heap(nums);
        // heap.ShowHeap();
        for (int i = 0; i < nums.size() - k; i++) {
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
    vector<int> nums = {0, 1, 2, 3, 4, 5, 6, 7};
    cout << "Should be 6" << Solution(nums, 2) << endl;

    nums = {1, 9, 2, 3, 73, 5, 6, 3};
    cout << "Should be 73" << Solution(nums, 1) << endl;

    nums = {0, 0, 0, 0, 1, 1, 1, 3, 4, 4};
    cout << "Should be 3" << Solution(nums, 3) << endl;

    nums = {3, 4, 4, 1, 0, 1, 0, 1, 0};
    cout << "Should be 3" << Solution(nums, 3) << endl;

    nums = {1};
    cout << "Should be 1" << Solution(nums, 1) << endl << "Fast" << endl;

    nums = {0, 1, 2, 3, 4, 5, 6, 7};
    cout << "Should be 7, result is " << SolFast(nums, 1) << endl;

    nums = {1, 9, 2, 3, 73, 5, 6, 3};
    cout << "Should be 5, result is " << SolFast(nums, 4) << endl;

    nums = {1, 9, 2, 3, 73, 5, 6, 3};
    cout << "Should be 3, result is " << SolFast(nums, 5) << endl;

    nums = {1, 9, 2, 3, 73, 5, 6, 3};
    cout << "Should be 3, result is " << SolFast(nums, 6) << endl;

    nums = {1, 9, 2, 3, 73, 5, 6, 3};
    cout << "Should be 2, result is " << SolFast(nums, 7) << endl;

    nums = {0, 0, 0, 0, 1, 1, 1, 3, 4, 4};
    cout << "Should be 4, result is " << SolFast(nums, 2) << endl;

    nums = {0, 0, 0, 0, 1, 1, 1, 3, 4, 4};
    cout << "Should be 3, result is " << SolFast(nums, 3) << endl;

    nums = {0, 0, 0, 0, 1, 1, 1, 3, 4, 4};
    cout << "Should be 1, result is " << SolFast(nums, 4) << endl;

    nums = {3, 4, 4, 1, 0, 1, 0, 1, 0};
    cout << "Should be 3, result is " << SolFast(nums, 3) << endl;

    nums = {1};
    cout << "Should be 1, result is " << SolFast(nums, 1) << endl;
    return;
}

int main() {
    test1();
    return 0;
}