#ifndef SORT_SEARCH_H
#define SORT_SEARCH_H

#include "DynamicArray.h"

template <typename T>
void bubbleSort(DynamicArray<T>& arr, bool (*compare)(const T&, const T&)) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (compare(arr[j + 1], arr[j])) {
                T temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

template <typename T>
void insertionSort(DynamicArray<T>& arr, bool (*compare)(const T&, const T&)) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && compare(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template <typename T, typename K>
int linearSearch(const DynamicArray<T>& arr, const K& key,
                 bool (*match)(const T&, const K&)) {
    for (int i = 0; i < arr.size(); i++) {
        if (match(arr[i], key)) return i;
    }
    return -1;
}

template <typename T, typename K>
DynamicArray<int> linearSearchAll(const DynamicArray<T>& arr, const K& key,
                                  bool (*match)(const T&, const K&)) {
    DynamicArray<int> results;
    for (int i = 0; i < arr.size(); i++) {
        if (match(arr[i], key)) {
            results.push_back(i);
        }
    }
    return results;
}

template <typename T, typename K>
int binarySearch(const DynamicArray<T>& arr, const K& key,
                 int (*compareWithKey)(const T&, const K&)) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = compareWithKey(arr[mid], key);
        if (cmp == 0) return mid;
        else if (cmp < 0) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

#endif
