#include <iostream>

#include "myStack.hpp"
using namespace std;

int* mySolution(int arr[], int length) {
    // 时间复杂度：O(N) (pop + push), but pop <= push < n
    // 空间复杂度：O(N)
    // 从右向左,从而避免重复扫描(若A在B左边且A >
    // B,在B右第一个比B大的元素的左边且A右边的那几个一定小于B所以小于A).
    // 栈顶是左边元素,从底到顶 单调递减.
    Stack<int> s;
    int* res = new int[length];
    int closest, p, tmp;
    for (int i = length - 1; i >= 0; i--) {
        p = 0;
        while (1) {
            p++;
            // cout << s.get_top() << endl;
            if (s.is_empty()) {
                // 说明这就是最大值了
                // s.push(arr[i]);
                s.push(i);
                res[i] = 0;
                break;
            }
            // if (s.get_top() > arr[i]) {
            if (arr[s.get_top()] > arr[i]) {
                // 栈顶大,可以入栈

                // 这个还是不行,只算了最近一个元素跳过的
                // if (p == 1) {
                //     res[i] = p;
                // } else {
                //     res[i] = p + res[i + 1] - 1;
                // }
                // s.push(arr[i]);
                // 但是把index存到栈里就行...
                res[i] = s.get_top() - i;
                s.push(i);
                break;
            } else {
                s.pop();
            }
        }
    }
    return res;
}

int* notMySolution(int* arr, int length) {
    Stack<int> s;
    int* res = new int[length];
    for (int i = 0; i < length; i++) {
        // 单调递减（栈顶最小）栈，在栈里的都是还没找到下个更大值的元素
        // 必须比栈顶小才能入栈，否则pop并可以知道对应的res
        while (!s.is_empty() && arr[s.get_top()] <= arr[i]) {
            // empty: arr[i] is larger than all elements in the
            // stack
            res[s.get_top()] = i - s.get_top();
            s.pop();
        }
        s.push(i);
    }
    // 如果还有剩下，就都是找不到的
    while (!s.is_empty()) {
        res[s.pop()] = 0;
    }
    return res;
}

void test() {
    int arr[9] = {80, 60, 55, 40, 45, 56, 47, 48, 61};
    int* res = notMySolution(arr, 9);

    // int arr[6] = {80, 70, 60, 50, 40, 30};
    // int* res = mySolution(arr, 6);

    // int arr[6] = {30, 40, 50, 60, 70, 80};
    // int* res = mySolution(arr, 6);

    // int arr[6] = {8, 8, 8, 8, 8, 8};
    // int* res = mySolution(arr, 6);

    // int arr[1] = {100};
    // int* res = mySolution(arr, 1);

    for (int i = 0; i < 9; i++) {
        cout << res[i] << "  ";
    }
    cout << endl;
}

int main() {
    test();
    return 0;
}