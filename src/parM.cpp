#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
using namespace std;

void print_array(int arr[], int length) {
    for (int i = 0; i < length; ++i) {
        cout << arr[i]<<' ';
    }
    cout << endl;
}

/* check_array check if the array after the bitonic sort is properly sorted */
void check_array(int arr[], int length) {
    for (int i = 1; i < length; ++i) {
        if(arr[i-1]>arr[i]){
            exit(-1); // aborting the program if the array is not sorted
        }
    }
}

void bitonic_merge(int arr[], int start, int length, bool direction) { 
    //if (length == 1) return;
    int mid = length / 2;
    for (int i = start; i < start + mid; ++i) {
        if ((arr[i] > arr[i + mid]) == direction) { // perform the swap in order to construct the sequence
            int temp = arr[i];
            arr[i]= arr[i+mid];
            arr[i+mid] = temp;
        }
    }
    if(mid == 1) // optimization: avoid a recursive call
        return;
    bitonic_merge(arr, start, mid, direction);
    bitonic_merge(arr, start + mid, mid, direction);
}

void parallel_bitonic_merge(int arr[], int start, int length, bool direction, int num_threads) {
    
    if (length == 1) {
        return;
    }
    int mid = length / 2;
    for (int i = start; i < start + mid; ++i) {
        if ((arr[i] > arr[i + mid]) == direction) {
            int temp = arr[i];
            arr[i] = arr[i + mid];
            arr[i + mid] = temp;
        }
    }

    if (num_threads > 1) { 
        int new_threads = num_threads / 2;
        thread t1(parallel_bitonic_merge, arr, start, mid, direction, new_threads); // optimized version with multithreading in the merge phase, same three structure of recursive call
        thread t2(parallel_bitonic_merge, arr, start + mid, mid, direction, num_threads - new_threads);
        t1.join();
        t2.join();
    } else {
        bitonic_merge(arr, start, length, direction);
    }
}

void bitonic_sort(int arr[], int start, int length, bool direction) {
    if (length == 1) return;
    int mid = length / 2;
    bitonic_sort(arr, start, mid, true); // ascending sequence 
    bitonic_sort(arr, start + mid, mid, false); // discending sequence 
    bitonic_merge(arr, start, length, direction); // create the bitonic sequence
}

// entry point function for the bitonic_sort
void parallel_bitonic_sort(int arr[], int start, int length, bool direction, int num_threads) {
    if (num_threads == 1) { // call the bitonic_sort and create the bitonic sequence 
        bitonic_sort(arr, start, length, direction);
    } else {
        int mid = length / 2;
        thread t1(parallel_bitonic_sort, ref(arr), start, mid, true, num_threads / 2); // the first thread call recursively the function with first half of the array
        thread t2(parallel_bitonic_sort, ref(arr), start + mid, mid, false, num_threads / 2);  // the second thread call recursively the function with second half of array
        t1.join();
        t2.join();
        
        parallel_bitonic_merge(arr, start, length, direction, num_threads);  // perform the bitonic sequence merge and finalize the sort
    }
}

int main(int argc, char **argv) {
    // Size is 2^21
    int arraySize = pow(2, atoi(argv[1]));  // Size of the array (2^21 elements
    int numThreads = atoi(argv[2]);  // Number of threads

    int *arr = new int[arraySize];

    srand(time(0));
    for (int i = 0; i < arraySize; i++) {
        arr[i] = rand() % 2097152;
    }

    auto start = chrono::high_resolution_clock::now();
    parallel_bitonic_sort(arr, 0, arraySize, true, numThreads);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    
    cout << elapsed.count() << endl;

    // Check if the array is sorted
    /*for (int i = 0; i < arraySize - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            cout << "Array is not sorted" << endl;
            break;
        }
    }*/

    
    delete[] arr;

    return 0;
}
