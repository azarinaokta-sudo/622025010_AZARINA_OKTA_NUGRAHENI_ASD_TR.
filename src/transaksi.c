#include "transaksi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initTransaksiList(TransaksiList *list) {
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}

static int ensureCapacity(TransaksiList *list) {
    if (list->count < list->capacity) return 1;
    int newCap = (list->capacity == 0) ? INITIAL_CAPACITY : list->capacity * 2;
    Transaksi *tmp = (Transaksi *)realloc(list->data, (size_t)newCap * sizeof(Transaksi));
    if (!tmp) return 0;
    list->data = tmp;
    list->capacity = newCap;
    return 1;
}

int addTransaksi(TransaksiList *list, Transaksi t) {
    if (!ensureCapacity(list)) return 0;
    list->data[list->count++] = t;
    return 1;
}

int deleteTransaksiById(TransaksiList *list, int id) {
    for (int i = 0; i < list->count; i++) {
        if (list->data[i].id == id) {
            for (int j = i; j < list->count - 1; j++) {
                list->data[j] = list->data[j + 1];
            }
            list->count--;
            return 1;
        }
    }
    return 0;
}

int nextTransaksiId(const TransaksiList *list) {
    int maxId = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->data[i].id > maxId) maxId = list->data[i].id;
    }
    return maxId + 1;
}

int loadTransaksi(TransaksiList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        Transaksi t;
        char *tok = strtok(line, "|");  if (!tok) continue; t.id = atoi(tok);
        tok = strtok(NULL, "|");        if (!tok) continue; snprintf(t.namaPelanggan,  MAX_STR, "%s", tok);
        tok = strtok(NULL, "|");        if (!tok) continue; snprintf(t.platNomor,      MAX_STR, "%s", tok);
        tok = strtok(NULL, "|");        if (!tok) continue; snprintf(t.jenisKerusakan, MAX_STR, "%s", tok);
        tok = strtok(NULL, "|");        if (!tok) continue; t.biaya = atof(tok);
        tok = strtok(NULL, "|");        if (!tok) continue; snprintf(t.tanggal, sizeof(t.tanggal), "%s", tok);
        addTransaksi(list, t);
    }
    fclose(f);
    return 1;
}

int saveTransaksi(const TransaksiList *list, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    for (int i = 0; i < list->count; i++) {
        const Transaksi *t = &list->data[i];
        fprintf(f, "%d|%s|%s|%s|%.2f|%s\n",
                t->id, t->namaPelanggan, t->platNomor,
                t->jenisKerusakan, t->biaya, t->tanggal);
    }
    fclose(f);
    return 1;
}

void printTransaksi(const Transaksi *t) {
    printf(YEL "%-5d" RST " " WHT "%-22s" RST " " CYN "%-12s" RST " " MAG "%-22s" RST " " GRN "Rp%10.2f" RST "  " BLU "%s" RST "\n",
           t->id, t->namaPelanggan, t->platNomor,
           t->jenisKerusakan, t->biaya, t->tanggal);
}

void freeTransaksiList(TransaksiList *list) {
    free(list->data);
    list->data = NULL;
    list->count = 0;
    list->capacity = 0;
}
