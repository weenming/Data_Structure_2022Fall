#include <iostream>

using namespace std;


int mySolutionQ1(int *arr, int len){
    /*
        时间复杂度：遍历整个数组1次, O(n)
        空间复杂度：只需要1个计数的数字, O(1)
    */
    // 代码
    int res;
    int count = 1;
    res = arr[0];
    // Boyer–Moore majority vote algorithm

    // arr[0] selected as res initially
    // if next one the same number, count += 1
    // otherwise, count -= 1
    // when count < 0, the current element is chosen as res.
    
    // Because the target number appears in the array for over n/2 times, it must be res in the end. 
    
    for(int i=1; i<len; i++){
        // when count < 0, the current element is chosen as res.
        if(res == 0){
            res = arr[i];
            count += 1;
        }
        else {
            if (arr[i] == res){
                // if next one is the same number, count += 1
                count += 1;
            }
            else{
                count -= 1;
            }
        }
    }
    return res;
}

int mySolutionQ2(int* arr, int len){
    // Time complexity: iterate 3 arrays sized len, O(n)
    // Space complexit: 1 aditional array sized len+1 is used, O(n)

    // mark the arr[i]-th element in array 'exist' as 1, so that iterating 'exist'
    // and the index of the element with value 0 is the missing number in arr.
    int exist[len+1];
    int res = -1;
    for(int i=0; i<len+1; i++){
        exist[i] = 0;
    }
    for(int i=0; i<len; i++){
        exist[arr[i]] = 1;
    }
    for(int i=0; i<len+1; i++){
        if(exist[i] == 0) {
            res = i;
        }
    }
    return res;
}

int mySolutionQ2Sum(int* arr, int len){
    // Time complexity: O(n)
    // Space complexity: O(1)
    
    // if the missing number were in arr, the sum of arr should be n(n+1)/2
    // subtracting n(n+1)/2 by the actual sum of arr to get the missing number.
    int sum = len*(len+1)/2;
    int sum_real = 0;
    for(int i=0; i<len; i++){
        sum_real += arr[i];
    }
    return sum - sum_real;
}

void test() {
    // test cases of Q1
    const int len = 9;
    int arr[] = {1, 1, 0, 1, 0, 0, 0, 0, 2};
    
    // const int len = 1;
    // int arr[len] = {1};

    // const int len = 3;
    // int arr[len] = {11, 23, 11};

    int res = mySolutionQ1(arr, len); 
    cout << res << endl;
    return ;
}

int main() {
    test();
    int len = 5;
    int res = 0;
    // test cases of Q2
    // int arr[len] = {3,2,1,4,5};
    int arr[len] = {2,1,3,4,0};
    // int arr[len] = {0,2,3,1,5};

    // int arr[len] = {0};

    // for (int i = 0; i < len; ++i) {
    //     cin >> arr[i];
    // }

    res = mySolutionQ2(arr, len); 

    cout << res << endl;
    system("pause");
    return 0;
}
