#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <thread>
#include <vector>
#include <cmath>
#include <fstream>

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
    int array_lengths[] = {64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536};
    int num_threads[] = {1, 2, 4, 8, 16, 32};
    int numTrials = 30; // Number of trials

    // Open a file to write the report
    ofstream report;
    report.open("./src/results/report.txt");
    report << "Format: Array size, Threads, Time\n";

    //for every number of threads
    for (int threads : num_threads) {
        //for every array length
        for (int n : array_lengths) {
            double total_time = 0;
            for (int trial = 0; trial < numTrials; trial++) {
                int *arr = new int[n];
                for (int i = 0; i < n; i++) {
                    arr[i] = rand() % 1000;
                }

                auto start = chrono::high_resolution_clock::now();
                bitonicSort(arr, n, threads);
                auto end = chrono::high_resolution_clock::now();
                chrono::duration<double> elapsed = end - start;
                total_time += elapsed.count();

                delete[] arr;
            }
            double avg_time = total_time / numTrials;
            report << n << " " << threads << " " << avg_time << "\n";
        }
    }

    report.close(); // Close the file
    return 0;
}
