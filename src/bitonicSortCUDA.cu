#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <cmath>
using namespace std;

/* 
in-device function to swap two elements in the array
*/
__device__ void swap(int *a, int *b){
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

/* 
bitonicKernel utilizes the GPU to perform a single sorting pass on an array. 
It accepts three inputs: the array to be sorted, 
the distance between elements that may be swapped based on a specific check, 
and the size of the sequence to be sorted.
*/
__global__ void bitonicKernel(int *arr, unsigned long long distance, unsigned long long subSequence_size) {
  unsigned long long i, xorCheck;

  // Thread identification and index calculation
  i = threadIdx.x + blockDim.x * blockIdx.x;

  // Calculate bitonic comparison element using XOR
  xorCheck = i ^ distance;

  // Check if current element needs to be compared (based on bitonic sequence)
  if (xorCheck > i) {
    // Check if current element is at the beginning of a subsequence
    if ((i & subSequence_size) == 0) {
      // Perform ascending comparison for the first half of the bitonic sequence
      if (arr[i] > arr[xorCheck]) {
        swap(&arr[i], &arr[xorCheck]);
      }
    } else {
      // Perform descending comparison for the second half of the bitonic sequence
      if (arr[i] < arr[xorCheck]) {
        swap(&arr[i], &arr[xorCheck]);
      }
    }
  }
}


/* 
bitonicSort sets up the GPU and calls the bitonicKernel function to sort an array.
*/
void bitonicSort(int *arr, unsigned long long array_size, int num_threads) {
  // Allocate memory on the GPU for the array
  int *cuda_arr;
  size_t size = array_size * sizeof(int);
  cudaMalloc((void**) &cuda_arr, size);

  // Copy the array from host to device memory
  cudaMemcpy(cuda_arr, arr, size, cudaMemcpyHostToDevice);

  // Calculate the block size for efficient thread utilization
  int block_dim = (array_size + num_threads - 1) / num_threads;

  // Variables for bitonic sort parameters (can be pre-calculated for efficiency)
  unsigned long long distance, subSequence_size;

  // Timing variables for performance measurement
  cudaEvent_t start, stop;
  float elapsed;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);
  cudaEventRecord(start, 0);  // Record start time

  // Nested loop for bitonic sort passes
  for (subSequence_size = 2; subSequence_size <= array_size; subSequence_size <<= 1) {
    for (distance = subSequence_size >> 1; distance > 0; distance >>= 1) {
      // Launch the bitonicKernel with appropriate block and thread configuration
      bitonicKernel<<<block_dim, num_threads>>>(cuda_arr, distance, subSequence_size);
    }
  }

  // Record end time and synchronize for accurate measurement
  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&elapsed, start, stop);
  cudaEventDestroy(start);
  cudaEventDestroy(stop);

  // Print elapsed time in milliseconds (assuming milliseconds desired)
  printf("%f,", elapsed / 1000);  // Adjust division factor based on desired unit

  // Copy the sorted array back from device to host memory
  cudaMemcpy(arr, cuda_arr, size, cudaMemcpyDeviceToHost);

  // Free the allocated GPU memory
  cudaFree(cuda_arr);
}

int main(int argc, char** argv) {
  // Check for valid program arguments
  if (argc != 3) {
    printf("Usage: %s <array_size> <num_threads>\n", argv[0]);
    return 1;
  }

  // Get array size from command-line argument
  int array_size = pow(2, atoi(argv[1]));
  if (array_size <= 0) {
    printf("Array size must be a positive power of 2\n");
    return 1;
  }

  // Get number of threads from command-line argument
  unsigned int num_threads = atoi(argv[2]);

  // Validate that num_threads is a power of 2
  if ((num_threads & (num_threads - 1)) != 0) {
    printf("The number of threads must be a power of 2\n");
    return 1;
  }

  // Allocate memory on host (CPU) for the array
  int *arr = new int[array_size];

  // Seed random number generator
  srand(time(0));

  // Initialize array with random values (0 to 2097151)
  for (int i = 0; i < array_size; i++) {
    arr[i] = rand() % 2097152;
  }

  // Timing setup for overall execution time (including memory transfers)
  cudaEvent_t start_exec, end_exec;
  float elapsed;
  cudaEventCreate(&start_exec);
  cudaEventCreate(&end_exec);
  cudaEventRecord(start_exec, 0);  // Record start time

  // Call bitonicSort function for sorting
  bitonicSort(arr, array_size, num_threads);

  // Record end time and synchronize for accurate measurement
  cudaEventRecord(end_exec, 0);
  cudaEventSynchronize(end_exec);
  cudaEventElapsedTime(&elapsed, start_exec, end_exec);
  cudaEventDestroy(start_exec);
  cudaEventDestroy(end_exec);

  // Print total execution time in milliseconds
  printf("%f\n", elapsed / 1000);

  // Deallocate host memory
  delete[] arr;

  return 0;
}