#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "config.h"

typedef struct Member {
    char platNomor[MAX_STR];
    char nama[MAX_STR];
    int  poin;
    struct Member *next;
} Member;

typedef struct {
    Member *buckets[HASH_SIZE];
    int     count;
} HashTable;

void         initHashTable(HashTable *ht);
unsigned int hashFunction(const char *key);
Member      *searchMember(HashTable *ht, const char *plat);

/* return: 1 sukses, 0 gagal alokasi, -1 duplikat */
int          insertMember(HashTable *ht, const char *plat, const char *nama, int poin);
int          deleteMember(HashTable *ht, const char *plat);
void         printHashTable(const HashTable *ht);
void         freeHashTable(HashTable *ht);
int          loadMembers(HashTable *ht, const char *filename);
int          saveMembers(const HashTable *ht, const char *filename);

#endif /* HASHTABLE_H */
