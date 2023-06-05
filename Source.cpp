#include <iostream>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#include <vector>
#include <omp.h>
#include <algorithm>
#include <iomanip>

using namespace std;

int partition(int arr[], int start, int end)
{
    int pivot = arr[start];
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }
    // Giving pivot element its correct position
    int pivotIndex = start + count;
    swap(arr[pivotIndex], arr[start]);
    // Sorting left and right parts of the pivot element
    int i = start, j = end;
    while (i < pivotIndex && j > pivotIndex) {
        while (arr[i] <= pivot) {
            i++;
        }
        while (arr[j] > pivot) {
            j--;
        }
        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
    return pivotIndex;
}
void elementary_quickSort(int arr[], int start, int end)
{
    // base case
    if (start >= end)
        return;
    // partitioning the array
    int p = partition(arr, start, end);
    // Sorting the left part
    elementary_quickSort(arr, start, p - 1);
    // Sorting the right part
    elementary_quickSort(arr, p + 1, end);
}

void parallel_quickSort_base_one(int arr[], int start, int end)
{
    // base case
    if (start >= end)
        return;
    // partition the array
    int p = partition(arr, start, end);
    // sort the left and right parts in parallel using tasks
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task
            parallel_quickSort_base_one(arr, start, p - 1);
            #pragma omp task
            parallel_quickSort_base_one(arr, p + 1, end);
        }
    }
}

void parallel_quickSort_base_variable(int arr[], int start, int end)
{
    // base case
    if (end - start <= 5000) {
        elementary_quickSort(arr,start,end);
        return;
    }
    // partition the array
    int p = partition(arr, start, end);
    // sort the left and right parts in parallel using tasks
    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            #pragma omp task
            parallel_quickSort_base_variable(arr, start, p - 1);
            #pragma omp task
            parallel_quickSort_base_variable(arr, p + 1, end);
        }
    }
}

const int ARRAY_SIZE = 200000;

int main() {

    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    // Create an array of size ARRAY_SIZE
    int numbers[ARRAY_SIZE];
    // Populate the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        numbers[i] = rand();
    }
    double normal_start = omp_get_wtime();
    elementary_quickSort(numbers, 0, ARRAY_SIZE - 1);
    double normal_end = omp_get_wtime();
    double normal_elapsed = normal_end - normal_start;
    
    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    // Populate the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        numbers[i] = rand();
    }

    double stl_start = omp_get_wtime();
    std::sort(std::begin(numbers), std::end(numbers));
    double stl_end = omp_get_wtime();
    double stl_elapsed = stl_end - stl_start;

    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    // Populate the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        numbers[i] = rand();
    }

    double parallel_start = omp_get_wtime();
    parallel_quickSort_base_one(numbers, 0, ARRAY_SIZE - 1);
    double parallel_end = omp_get_wtime();
    double parallel_elapsed = parallel_end - parallel_start;

    // Seed the random number generator with the current time
    srand(static_cast<unsigned int>(time(0)));
    // Populate the array with random numbers
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        numbers[i] = rand();
    }

    double parallel_start_variable = omp_get_wtime();
    parallel_quickSort_base_variable(numbers, 0, ARRAY_SIZE - 1);
    double parallel_end_variable = omp_get_wtime();
    double parallel_elapsed_variable = parallel_end_variable - parallel_start_variable;

    // Print the elapsed time for each function
    std::cout << "Time taken for elementary_quicksort:                        " << normal_elapsed << std::endl;
    std::cout << "Time taken for stl_quicksort:                               " << stl_elapsed << std::endl;
    std::cout << "Time taken for parallel_quicksort with base case of 1:      " << parallel_elapsed << std::endl;
    std::cout << "Time taken for parallel_quicksort with base case of 1000:   " << parallel_elapsed_variable << std::endl;
    return 0;
}
