#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include "/opt/amduprof/include/AMDProfileController.h"


using namespace std;

void bitonicMerge(int arr[], int start, int length, bool direction) {
    int mid = length / 2;
    for (int i = start; i < start + mid; ++i) {
        if ((arr[i] > arr[i + mid]) == direction) {
            swap(arr[i], arr[i + mid]);
        }
    }
}

void bitonicSortSerial(int *arr, int low, int count, bool direction) {
    if (count > 1) {
        int k = count / 2;
        bitonicSortSerial(arr, low, k, true);
        bitonicSortSerial(arr, low + k, k, false);
        bitonicMerge(arr, low, count, direction);
    }
}

void bitonicSortParallel(int *arr, int low, int count, bool direction, int threads) {

    if (threads == 1 ||count <= 2048) {
        // If array length is below threshold, execute serial sort
        bitonicSortSerial(arr, low, count, direction);
        return;
    }

    else {
        int k = count / 2;

            // Use parallelism
            thread t1(bitonicSortParallel, arr, low, k, true, threads / 2);
            thread t2(bitonicSortParallel, arr, low + k, k, false, threads - threads / 2);
            t1.join();
            t2.join();
            bitonicMerge(arr, low, count, direction);
            
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

    amdProfileResume();
    bitonicSortParallel(arr, 0, arraySize, true, numThreads);
    amdProfilePause();


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
