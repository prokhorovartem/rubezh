#include <iostream>
#include "ThreadPool.h"

using namespace std;

#define THREADS_COUNT 4
#define MAX 200
int a[MAX];
int part = 0;

void merge(int low, int mid, int high)
{
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];

    int n1 = mid - low + 1,
            n2 = high - mid, i, j;

    for (i = 0; i < n1; i++)
        left[i] = a[i + low];

    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];

    int k = low;
    i = j = 0;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    while (i < n1) {
        a[k++] = left[i++];
    }

    while (j < n2) {
        a[k++] = right[j++];
    }
}

void merge_sort(int low, int high)
{
    int mid = low + (high - low) / 2;
    if (low < high) {

        merge_sort(low, mid);

        merge_sort(mid + 1, high);

        merge(low, mid, high);
    }
}

void merge_sort_threads()
{
    int thread_part = part++;

    int low = thread_part * (MAX / THREADS_COUNT);
    int high = (thread_part + 1) * (MAX / THREADS_COUNT) - 1;

    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }
}


int main() {
    ThreadPool pool(THREADS_COUNT);
    pool.init();

    for (int & i : a) {
        int num = rand() % 1000;
        i = num;
        cout << i << " ";
    }

    for (int i = 0; i < THREADS_COUNT; ++i) {
        pool.add_task(merge_sort_threads);
    }

    pool.shutdown();

    merge(0, (MAX / 2 - 1) / 2, MAX / 2 - 1);
    merge(MAX / 2, MAX/2 + (MAX-1-MAX/2)/2, MAX - 1);
    merge(0, (MAX - 1)/2, MAX - 1);

    cout << "\n";
    for (int i : a) {
        cout << i << " ";
    }
    return 0;
}


