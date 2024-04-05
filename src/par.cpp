#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <thread>
#include <vector>
#include <cmath>

using namespace std;

void bitonicMerge(int *arr, int low, int count, bool direction) {
    if (count > 1) {
        int k = count / 2;
        for (int i = low; i < low + k; i++) {
            if ((arr[i] > arr[i + k]) == direction) {
                swap(arr[i], arr[i + k]);
            }
        }
        bitonicMerge(arr, low, k, direction);
        bitonicMerge(arr, low + k, k, direction);
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
    if (count > 1) {
        int k = count / 2;
        if (threads > 1 && count ) {
            // Use parallelism
            std::vector<std::thread> thread_pool;
            thread_pool.emplace_back(std::thread(bitonicSortParallel, arr, low, k, true, threads / 2));
            thread_pool.emplace_back(std::thread(bitonicSortParallel, arr, low + k, k, false, threads / 2));

            for (auto& t : thread_pool) {
                t.join();
            }
        } else {
            bitonicSortSerial(arr, low, k, true);
            bitonicSortSerial(arr, low + k, k, false);
        }
        bitonicMerge(arr, low, count, direction);
    }
}

void bitonicSort(int *arr, int n, int threads) {
    bitonicSortParallel(arr, 0, n, true, threads);
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

    bitonicSort(arr, arraySize, numThreads);

    /*// Check if the array is sorted
    for (int i = 0; i < arraySize - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            cout << "Array is not sorted" << endl;
            break;
        }
    }
    */
    delete[] arr;
    
   printf("Done\n");
    return 0;
}
