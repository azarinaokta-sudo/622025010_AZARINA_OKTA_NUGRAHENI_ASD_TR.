#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initHashTable(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
}

/* Fungsi hash djb2 */
unsigned int hashFunction(const char *key) {
    unsigned int hash = 5381;
    while (*key) {
        hash = ((hash << 5) + hash) + (unsigned char)(*key);
        key++;
    }
    return hash % HASH_SIZE;
}

Member *searchMember(HashTable *ht, const char *plat) {
    Member *cur = ht->buckets[hashFunction(plat)];
    while (cur) {
        if (strcmp(cur->platNomor, plat) == 0) return cur;
        cur = cur->next;
    }
    return NULL;
}

int insertMember(HashTable *ht, const char *plat, const char *nama, int poin) {
    if (searchMember(ht, plat)) return -1;

    Member *m = (Member *)malloc(sizeof(Member));
    if (!m) return 0;

    snprintf(m->platNomor, MAX_STR, "%s", plat);
    snprintf(m->nama, MAX_STR, "%s", nama);
    m->poin = poin;

    unsigned int idx = hashFunction(plat);
    m->next = ht->buckets[idx];
    ht->buckets[idx] = m;
    ht->count++;
    return 1;
}

int deleteMember(HashTable *ht, const char *plat) {
    unsigned int idx = hashFunction(plat);
    Member *cur = ht->buckets[idx];
    Member *prev = NULL;

    while (cur) {
        if (strcmp(cur->platNomor, plat) == 0) {
            if (prev) prev->next = cur->next;
            else      ht->buckets[idx] = cur->next;
            free(cur);
            ht->count--;
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

void printHashTable(const HashTable *ht) {
    printf(YEL BOLD "Total member: %d" RST "\n", ht->count);
    for (int i = 0; i < HASH_SIZE; i++) {
        Member *cur = ht->buckets[i];
        if (!cur) continue;
        printf(CYN "Bucket %2d: " RST, i);
        while (cur) {
            printf(WHT "[%s - %s " GRN "(%d poin)" WHT "]" RST,
                   cur->platNomor, cur->nama, cur->poin);
            if (cur->next) printf(" -> ");
            cur = cur->next;
        }
        printf("\n");
    }
}

void freeHashTable(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Member *cur = ht->buckets[i];
        while (cur) {
            Member *nx = cur->next;
            free(cur);
            cur = nx;
        }
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
}

int loadMembers(HashTable *ht, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        char *tok = strtok(line, "|"); if (!tok) continue;
        char plat[MAX_STR]; snprintf(plat, MAX_STR, "%s", tok);
        tok = strtok(NULL, "|");       if (!tok) continue;
        char nama[MAX_STR]; snprintf(nama, MAX_STR, "%s", tok);
        tok = strtok(NULL, "|");       if (!tok) continue;

        insertMember(ht, plat, nama, atoi(tok));
    }
    fclose(f);
    return 1;
}

int saveMembers(const HashTable *ht, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    for (int i = 0; i < HASH_SIZE; i++) {
        Member *cur = ht->buckets[i];
        while (cur) {
            fprintf(f, "%s|%s|%d\n", cur->platNomor, cur->nama, cur->poin);
            cur = cur->next;
        }
    }
    fclose(f);
    return 1;
}
