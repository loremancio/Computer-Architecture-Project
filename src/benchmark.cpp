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
        if (threads > 1) {
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
    if (threads == 1) {
        bitonicSortSerial(arr, 0, n, true);
    } else {
        bitonicSortParallel(arr, 0, n, true, threads);
    }
}

int main() {
    int len[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
    int threads[] = {1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22};
    int numTrials = 30; // Number of trials

    FILE* report = fopen("report.txt", "a");
    fprintf(report, "Format: Array size, Threads, Time\n");

    for (int trial = 0; trial < numTrials; trial++) {
        for (int i = 0; i < 11; i++) {
            for (int j = 0; j < 12; j++) {
                int arraySize = len[i];
                int numThreads = threads[j];
                int *arr = new int[arraySize];
                srand(time(0));
                for (int i = 0; i < arraySize; i++) {
                    arr[i] = rand() % 1000;
                }
                auto start = chrono::high_resolution_clock::now();
                bitonicSort(arr, arraySize, numThreads);
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double> elapsed = end - start;

                for (int i = 0; i < arraySize - 1; i++) {
                    if (arr[i] > arr[i + 1]) {
                        cout << "Array is not sorted" << endl;
                        delete[] arr;
                        return 0;
                    }
                }
                delete[] arr;

                //cout << "Array size: " << arraySize << " Threads: " << numThreads << " Time: " << elapsed.count() << endl;

                FILE* report = fopen("report.txt", "a");
                fprintf(report, "%d, %d, %f\n", arraySize, numThreads, elapsed.count());
            }
        }
        cout << "Trial " << trial + 1 << " completed" << endl;
        
    }

    return 0;
}



