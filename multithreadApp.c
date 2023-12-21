#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <thread>
#include <mutex>
//include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctime>

struct range
{
    int low;
    int high;
};

int n, s, r, p, t;
char* a, * m, * m3;
int* arr = NULL;
struct range* ranges = NULL;
clock_t startTime, endTime;
struct timeval startTimeOfDay, endTimeOfDay;

// Main Method
int main(int argc, char* argv[])
{   
    printf("");
    if (checkForErrors() == 1)
    {
        printf("\n\nAn error was found in the arguments.\n\n"); // Prints error message
        return 1;
    }
    arr = malloc(n * sizeof(int));
    ranges = malloc(p * sizeof(struct range));
    createArray();
    shuffle();
}

//Reusable Functions for multithreading
int checkForErrors()
{
    // Checks if Size n < 1
    if (n < 1)
    {
        return 1;
    }
    // Checks if Max Threads t > Pieces p
    else if (t > p)
    {
        return 1;
    }
    return 0;
}

// Runs a standard insertion sort that passes an array and sorts only between the low and high indexes
void insertionSort(int arr[], int high, int low)
{
    int i, j, currValue;
    for (i = low + 1; i <= high; i++)
    {
        currValue = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > currValue)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = currValue;
    }
}

// Create Array (fills)
void createArray()
{
    for (int i = 0; i < n; i++)
    {
        arr[i] = i;
    }
}

// Shuffle Array
void shuffle()
{
    // Checks seed specifications
    if (r == -1)
    {
        srand(clock());
    }
    else if (r < -2 || r >= 0)
    {
        srand(r);
    }
    // Shuffles the array
    for (int i = 0; i < n; i++)
    {
        int j = rand() % n;
        int hold = arr[i];
        arr[i] = arr[j];
        arr[j] = hold;
    }
}

// Checks if array is sorted correctly 
int isSorted()
{
    bool hasError = false;
    for (int i = 0; i < n; i++)
    {
        // Checks for errors in the array
        // if (i != arr[i])
        if ((arr[i] + 1 != arr[i + 1]) && i != n - 1)
        {
            printf("\nThe list is not sorted... at index[%d]... the size of array is: %d", i, n);
            hasError = true;
        }
    }
    if (hasError)
    {
        return 1;
    }
    // Prints this if no errors were found
    printf("The list is sorted!\n");
    return 0;
}

// Runner function
void* runner(void* parameter)
{
    // Performs the sort between low and high indexes then exits thread
    struct range* rangeParameter = (struct range*)parameter;
    quickSort(rangeParameter->high, rangeParameter->low);
    pthread_exit(0);
}

// // Runs a Hibbard's sequence shell sort that passes an array and sorts only between the low and high indexes
void shellSort(int high, int low)
{
    int k = 1;
    int size = high - low + 1;
    while (size >= k)
    {
        k *= 2;
    }
    k = (k / 2) - 1;
    do
    {
        for (int i = low; i < (high - (k - 1)); i++)
        {

            for (int j = i; j >= 0; j -= k)
            {
                if (arr[j] <= arr[j + k])
                {
                    break;
                }
                else
                {
                    int temp = arr[j];
                    arr[j] = arr[j + k];
                    arr[j + k] = temp;
                }
            }
        }
        k = k >> 1;
    } while (k > 0);
}

// Hybrid Function
void quickSort(int high, int low)
{
    // Errors if high or low are < 0
    if (high < 0 || low < 0)
    {
        printf("\n\n!!!Number less than zero passed!!!\n\n");
        printf("High: %d\nLow: %d\n\n", high, low);
    }
    int pSize = high - low + 1;

    // Returns since array is already sorted
    if (pSize < 2)
    {
        return;
    }

    // Compare and swap values if necessary
    else if (pSize == 2)
    {
        if (arr[low] > arr[high])
        {
            int temp = arr[low];
            arr[low] = arr[high];
            arr[high] = temp;
            return;
        }
    }

    // Use shell sort
    else if (pSize > 2 && pSize <= s && (strcmp(a, "S") == 0 || strcmp(a, "s") == 0))
    {
        shellSort(high, low);
    }

    // Use insertion sort
    else if (pSize > 2 && pSize <= s && (strcmp(a, "I") == 0 || strcmp(a, "i") == 0))
    {
        insertionSort(arr, high, low);
    }

    // Recursively call QuickSort to both sides of partition
    else if (pSize > s)
    {
        int currentIdx = 0;
        bool isM3 = false;

        int pivotIndex = 0;
        int pivot;
        if ((strcmp(m3, "y") == 0 || strcmp(m3, "Y") == 0)) // Use median-of-3 for pivot
        {
            isM3 = true;
            int arrm3[3] = { arr[low], arr[(high - low) / 2 + low], arr[high] };
            insertionSort(arrm3, 2, 0);
            pivot = arrm3[1];
        }
        else
        {
            pivot = arr[high]; // Uses high index as pivot
        }

        int i = (low - 1);

        for (int j = low; j <= high; j++)
        {
            // If current element is smaller than the pivot
            if (arr[j] < pivot)
            {
                i++;

                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        // If Median of 3 is enabled
        if (isM3)
        {
            // Finds index of the pivot
            for (int x = low; x <= high; x++)
            {
                if (arr[x] == pivot)
                {
                    pivotIndex = x;
                    break;
                }
            }

            // Swaps pivot and i+1
            int temp2 = arr[i + 1];
            arr[i + 1] = arr[pivotIndex];
            arr[pivotIndex] = temp2;
            pivotIndex = i + 1;
        }
        else
        {
            // Swaps pivot (on the far right) and i+1
            int temp2 = arr[i + 1];
            arr[i + 1] = arr[high];
            arr[high] = temp2;
            pivotIndex = i + 1;
        }

        // Calls quicksort on lower half and upper half of partition
        quickSort(pivotIndex - 1, low);
        quickSort(high, pivotIndex + 1);
    }
}

// Partitioning Function to split array into p pieces
int partition(int low, int high)
{
    int currentIdx = 0;
    bool isM3 = false;

    int pivotIndex = 0;
    int pivot;
    if ((strcmp(m3, "y") == 0 || strcmp(m3, "Y") == 0)) // Use median-of-3 for pivot
    {
        isM3 = true;
        int arrm3[3] = { arr[low], arr[(high - low) / 2 + low], arr[high] };
        insertionSort(arrm3, 2, 0);
        pivot = arrm3[1];
    }
    else
    {
        pivot = arr[high]; // Uses high index as pivot
    }

    int i = (low - 1); // Index of smaller element and indicates the
    // right position of pivot found so far

    for (int j = low; j <= high; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element

            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    // If Median of 3 is enabled 
    if (isM3)
    {
        // Finds index of pivot
        for (int x = low; x <= high; x++)
        {
            if (arr[x] == pivot)
            {
                pivotIndex = x;
                break;
            }
        }

        // Swaps pivot and i+1
        int temp2 = arr[i + 1];
        arr[i + 1] = arr[pivotIndex];
        arr[pivotIndex] = temp2;
        pivotIndex = i + 1;
    }
    else
    {
        // Swaps pivot (on the far right) and i+1
        int temp2 = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp2;
        pivotIndex = i + 1;
    }
    return pivotIndex;
}
