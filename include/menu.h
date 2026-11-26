#ifndef MENU_H
#define MENU_H

#include "auth.h"
#include "transaksi.h"
#include "queue_stack.h"
#include "hashtable.h"
#include "bst.h"
#include "graph.h"

typedef struct {
    AccountList   accounts;
    TransaksiList transaksiList;
    Queue         antreanServis;
    Stack         riwayatStack;
    HashTable     memberTable;
    BSTNode      *katalogRoot;
    Graph         petaGraph;
    int           nextId;
} AppState;

void        detectDataDir(void);
const char *getDataPath(const char *filename);

int         menuAuth(AppState *app, char *usernameOut, int size);
void        menuUtama(AppState *app, const char *username);
void        layarPerpisahan(void);

#endif /* MENU_H */
