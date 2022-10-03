#include <stdio.h>
#include <stdlib.h>
int binarySearchIndex(int, int*, int);
int searchIndex(int, int*, int);

void mySort(int *arr, int len) {
    // 这里写排序算法
	int min = arr[0];
    int min_index = 0;
	// Iterate the array len times, each time move the minimum
	// element to the left end of the unsorted part (sorted elements
	// in ascending order, stored in the same array)
	for(int i=0; i<len; i++){
    	for(int j=i+1; j<len; j++){
			if (arr[j] < min){
				min = arr[j];
				min_index = j;
				// printf("j= %d\n", j);
			}
		}
		
		arr[min_index] = arr[i];
		arr[i] = min;
		
		min = arr[i+1];
		min_index = i+1;
	}
	return;
}

void mySortInsert(int* arr, int len){
	// use insertion sort, so that binary search can be used
	// Have not realized O(nlogn) time comlexity
	int insert_index;
	int sorted[len];
	sorted[0] = arr[0];
	for(int i=1; i<len; i++){
		// THIS HAS NOT BEEN DEBUGGED
		insert_index = binarySearchIndex(arr[i], sorted, i);
		for(int j=i-1; j>insert_index; j--){
			// Here, rearranging the array requires O(n^2) time complexity
			// Are there other ways to reduce the time? I can only think of using a linked list
			sorted[j+1] = sorted[j];			
		}
		sorted[insert_index] = arr[i];
	}
	// copy sorted to arr
	// If I did not use another array to store the sorted array, the performance might be better (?)
	for(int i=0; i<len;i++){
		arr[i]=sorted[i];
	}
	return;
}

int searchIndex(int insert_element, int* sorted, int sorted_len){
	if (insert_element < sorted[0]){
		return 0;
	}
	else if (insert_element > sorted[sorted_len-1]){
		return sorted_len - 1;
	}
	else{
		int i=0;
		while(i++ < sorted_len){
			if(insert_element < sorted[i+1] && insert_element > sorted[i]){
				return i;
			}
		}
	}
	return 0;
}

int binarySearchIndex(int insert_element, int* sorted, int sorted_len){
	if (insert_element < sorted[0]){
		return 0;
	}
	else if (insert_element > sorted[sorted_len-1]){
		return sorted_len - 1;
	}
	else{
		int left=0;
		int right=sorted_len-1;
		int middle;
		while(left+1 < right){
			middle = (left + right)/2;
			if(sorted[middle] > insert_element){
				right = middle;
			}
			else if(sorted[middle] < insert_element){
				left = middle;
			}
			else{
				return middle;
			}
		}
	}
	return 0 ;
}



void test() {
    // 这里写测试用例，如
    const int len = 7;
    int arr[len] = {1, 103, 3, 5, 2, 4, 9};
	// int arr[len] = {0, 23, 13, 14, 13};
	// int arr[len] = {1};

    mySortInsert(arr, len); 
    for (int i = 0; i < len; ++i) {  
        printf("%d ", arr[i]);
    }
    printf("\n");
    return;
}

int main() {
    test();
//    int len = 0;
//    scanf("%d", &len); 
//    int *arr = (int *)malloc(sizeof(int) * len); 
//    for (int i = 0; i < len; ++i) {
//        scanf("%d", &arr[i]);
//    }
//    mySort(arr, len); 

//    for (int i = 0; i < len; ++i) {
//        printf("%d ", arr[i]);
//    }
//    printf("\n");
	system("pause");
    return 0;
}
