#include <iostream>
#include <vector>

#include "MyHeap.hpp"

using namespace std;

int Solution(vector<int> &nums) {
    // find x_i so that \sum_j \sum_{i > j} x_i is minimized
    // or, (n-1)(x_i + x_j) + (n-2)x_k + (n-3)x_l + ... + 2x_{whatever} + +
    // x_{another whatever}
    // Thus, sort and add together. Use heap sorting to achieve time complexity
    // of O(nlogn)
    // Space complexity is O(n)

    if (nums.size() <= 1) {
        cout << "invalid nums" << endl;
        return 0;
    }

    MyHeap<int> heap(nums);
    int res, n = nums.size(), i = 1;
    // smallest, multiply by (n-1), and so does the second smallest

    res = heap.PopTop() * (n - 1);

    while (!heap.IsEmpty()) {
        res += heap.PopTop() * (n - i);
        i++;
    }
    return res;
}

void test1() {
    // vector<int> nums;
    // nums.push_back(2);
    // nums.push_back(4);
    // nums.push_back(3);

    vector<int> nums = {1, 2, 3, 4, 5};

    // vector<int> nums = {1, 2};

    // vector<int> nums = {1};

    cout << Solution(nums) << endl;
    return;
}

int main() {
    test1();
    return 0;
}