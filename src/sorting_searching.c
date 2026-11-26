#include "sorting_searching.h"
#include <string.h>

static int compareTransaksi(const Transaksi *a, const Transaksi *b, SortKey key) {
    switch (key) {
        case SORT_BY_ID:    return a->id - b->id;
        case SORT_BY_BIAYA: return (a->biaya > b->biaya) - (a->biaya < b->biaya);
        case SORT_BY_NAMA:  return strcmp(a->namaPelanggan, b->namaPelanggan);
    }
    return 0;
}

/* Algoritma 1: Selection Sort */
void selectionSort(Transaksi *arr, int n, SortKey key, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        int idx = i;
        for (int j = i + 1; j < n; j++) {
            int c = compareTransaksi(&arr[j], &arr[idx], key);
            if ((ascending && c < 0) || (!ascending && c > 0)) {
                idx = j;
            }
        }
        if (idx != i) {
            Transaksi tmp = arr[i];
            arr[i] = arr[idx];
            arr[idx] = tmp;
        }
    }
}

/* Algoritma 2: Insertion Sort */
void insertionSort(Transaksi *arr, int n, SortKey key, int ascending) {
    for (int i = 1; i < n; i++) {
        Transaksi k = arr[i];
        int j = i - 1;
        while (j >= 0) {
            int c = compareTransaksi(&arr[j], &k, key);
            if ((ascending && c > 0) || (!ascending && c < 0)) {
                arr[j + 1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j + 1] = k;
    }
}

/* Sequential Search berdasarkan nama pelanggan */
int sequentialSearchNama(const Transaksi *arr, int n, const char *nama) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].namaPelanggan, nama) == 0) return i;
    }
    return -1;
}

/* Binary Search berdasarkan ID (array wajib terurut ascending by ID) */
int binarySearchId(const Transaksi *arr, int n, int id) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid].id == id) return mid;
        if (arr[mid].id < id) lo = mid + 1;
        else                  hi = mid - 1;
    }
    return -1;
}
