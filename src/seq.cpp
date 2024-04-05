#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>

using namespace std;


void bitonicSort(int *arr, int n) {
    int k, j, l, i, temp;
    for (k = 2; k <= n; k *= 2) {
        for (j = k/2; j > 0; j /= 2) {
            for (i = 0; i < n; i++) {
                l = i ^ j;
                if (l > i) {
                    if ( ((i&k)==0) && (arr[i] > arr[l]) || ( ( (i&k)!=0) && (arr[i] < arr[l])) )  {
                        temp = arr[i];
                        arr[i] = arr[l];
                        arr[l] = temp;
                    }
                }
            }
        }
    }
}

int main() {
    //size is 2^21
    int arraySize = 16;

    int *arr = new int[arraySize];

    for (int i = 0; i < arraySize; i++) {
        arr[i] = rand() % 2097152;
    }

    auto start = chrono::high_resolution_clock::now();
    bitonicSort(arr, arraySize);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;
    cout << "Time: " << diff.count() << " s" << endl;

    //check if the array is sorted
    for (int i = 0; i < arraySize - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            cout << "Array is not sorted" << endl;
            break;
        }
    }

    for (int i = 0; i < arraySize; i++){
        cout << "[" << arr[i] << "]";
    }

    delete[] arr;
    return 0;
}