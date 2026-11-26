#ifndef TRANSAKSI_H
#define TRANSAKSI_H

#include "config.h"

typedef struct {
    int    id;
    char   namaPelanggan[MAX_STR];
    char   platNomor[MAX_STR];
    char   jenisKerusakan[MAX_STR];
    double biaya;
    char   tanggal[16];
} Transaksi;

typedef struct {
    Transaksi *data;
    int        count;
    int        capacity;
} TransaksiList;

void initTransaksiList(TransaksiList *list);
int  addTransaksi(TransaksiList *list, Transaksi t);
int  deleteTransaksiById(TransaksiList *list, int id);
int  nextTransaksiId(const TransaksiList *list);
int  loadTransaksi(TransaksiList *list, const char *filename);
int  saveTransaksi(const TransaksiList *list, const char *filename);
void printTransaksi(const Transaksi *t);
void freeTransaksiList(TransaksiList *list);

#endif /* TRANSAKSI_H */
