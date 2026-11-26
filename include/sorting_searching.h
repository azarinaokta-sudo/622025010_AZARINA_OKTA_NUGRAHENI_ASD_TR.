#ifndef SORTING_SEARCHING_H
#define SORTING_SEARCHING_H

#include "transaksi.h"

typedef enum {
    SORT_BY_ID,
    SORT_BY_BIAYA,
    SORT_BY_NAMA
} SortKey;

/* Algoritma Sorting (ditulis mandiri tanpa qsort) */
void selectionSort(Transaksi *arr, int n, SortKey key, int ascending);
void insertionSort(Transaksi *arr, int n, SortKey key, int ascending);

/* Algoritma Searching (ditulis mandiri tanpa bsearch) */
int  sequentialSearchNama(const Transaksi *arr, int n, const char *nama);
int  binarySearchId(const Transaksi *arr, int n, int id);

#endif /* SORTING_SEARCHING_H */
