// Sistem Manajemen Operasional & Layanan Tambal Ban Panggilan
// Tugas Rancang Algoritma dan Struktur Data (ASD) 2026
//
// Kompilasi (GCC / MinGW-w64):
//   gcc -std=c11 -Wall -Wextra -Iinclude src/*.c -o app

#include <stddef.h>
#include "config.h"
#include "auth.h"
#include "transaksi.h"
#include "queue_stack.h"
#include "hashtable.h"
#include "bst.h"
#include "graph.h"
#include "ui.h"
#include "menu.h"

int main(void) {
    AppState app;
    app.katalogRoot = NULL;
    app.nextId = 1;

    initTerminal();
    initAccounts(&app.accounts);
    initTransaksiList(&app.transaksiList);
    initQueue(&app.antreanServis);
    initStack(&app.riwayatStack);
    initHashTable(&app.memberTable);
    initGraph(&app.petaGraph);

    detectDataDir();
    loadAccounts (&app.accounts,      getDataPath("akun.txt"));
    loadTransaksi(&app.transaksiList, getDataPath("transaksi.txt"));
    loadMembers  (&app.memberTable,   getDataPath("member.txt"));
    loadKatalog  (&app.katalogRoot,   getDataPath("katalog.txt"));
    loadPeta     (&app.petaGraph,     getDataPath("peta.txt"));
    app.nextId = nextTransaksiId(&app.transaksiList);

    /* Akun bawaan bila file akun belum ada / kosong */
    if (app.accounts.count == 0) {
        registerAccount(&app.accounts, "admin", "admin123");
    }

    char currentUser[MAX_STR] = "";
    while (menuAuth(&app, currentUser, MAX_STR)) {
        menuUtama(&app, currentUser);
    }

    saveAccounts (&app.accounts,      getDataPath("akun.txt"));
    saveTransaksi(&app.transaksiList, getDataPath("transaksi.txt"));
    saveMembers  (&app.memberTable,   getDataPath("member.txt"));
    saveKatalog  (app.katalogRoot,    getDataPath("katalog.txt"));
    savePeta     (&app.petaGraph,     getDataPath("peta.txt"));

    freeTransaksiList(&app.transaksiList);
    freeHashTable    (&app.memberTable);
    freeBST          (app.katalogRoot);
    freeGraph        (&app.petaGraph);

    layarPerpisahan();
    return 0;
}
