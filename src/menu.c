#include "menu.h"
#include "ui.h"
#include "sorting_searching.h"
#include <stdio.h>
#include <string.h>

static char dataDir[16] = "data/";

void detectDataDir(void) {
    static const char *cand[] = { "data/", "", "../data/" };
    char path[64];
    for (int i = 0; i < 3; i++) {
        snprintf(path, sizeof(path), "%sakun.txt", cand[i]);
        FILE *f = fopen(path, "r");
        if (f) {
            fclose(f);
            snprintf(dataDir, sizeof(dataDir), "%s", cand[i]);
            return;
        }
    }
    dataDir[0] = '\0';
}

const char *getDataPath(const char *filename) {
    static char buf[4][128];
    static int k = 0;
    k = (k + 1) % 4;
    snprintf(buf[k], sizeof(buf[k]), "%s%s", dataDir, filename);
    return buf[k];
}

/* ----- Submenu 1: Antrean (Queue) & Riwayat (Stack) ----- */
static void aksiAntreanTambah(AppState *app) {
    Transaksi t;
    screenTitle("TAMBAH KENDARAAN KE ANTREAN", GRN);
    t.id = app->nextId;
    msgInfo("Nomor transaksi: %d", t.id);
    printf("\n");
    if (!inputTeks("Nama Pelanggan    : ", t.namaPelanggan,  MAX_STR)) return;
    if (!inputTeks("Plat Nomor        : ", t.platNomor,      MAX_STR)) return;
    if (!inputTeks("Jenis Kerusakan   : ", t.jenisKerusakan, MAX_STR)) return;
    if (!inputDouble("Estimasi Biaya (Rp): ", &t.biaya)) {
        msgErr("Biaya tidak valid! Isi angka tidak negatif.");
        return;
    }
    getToday(t.tanggal, sizeof(t.tanggal));
    if (enqueue(&app->antreanServis, t)) {
        msgOk("Ditambahkan ke antrean (ID %d).", t.id);
        app->nextId++;
    } else {
        msgErr("Antrean penuh!");
    }
}

static void aksiAntreanProses(AppState *app) {
    Transaksi t;
    screenTitle("PROSES SERVIS", CYN);
    if (!dequeue(&app->antreanServis, &t)) {
        msgErr("Antrean kosong!");
        return;
    }
    Member *m = searchMember(&app->memberTable, t.platNomor);
    if (m) {
        msgInfo("Member terdeteksi: %s. Diskon 10%%! (+10 poin)", m->nama);
        t.biaya *= 0.9;
        m->poin += 10;
    }
    if (!addTransaksi(&app->transaksiList, t)) {
        msgErr("Gagal menyimpan transaksi!");
        return;
    }
    if (!push(&app->riwayatStack, t)) {
        msgInfo("Riwayat penuh, undo untuk transaksi ini tidak tersedia.");
    }
    msgOk("Servis selesai: ID %d (%s), total Rp%.2f", t.id, t.namaPelanggan, t.biaya);
}

static void aksiAntreanLihat(const AppState *app) {
    screenTitle("DAFTAR ANTREAN (FIFO)", MAG);
    printQueue(&app->antreanServis);
}

static void aksiAntreanRiwayat(const AppState *app) {
    screenTitle("RIWAYAT SERVIS (STACK / LIFO)", BLU);
    printStack(&app->riwayatStack);
}

static void aksiAntreanUndo(AppState *app) {
    Transaksi t;
    screenTitle("UNDO TRANSAKSI TERAKHIR", RED);
    if (!pop(&app->riwayatStack, &t)) {
        msgErr("Riwayat kosong!");
        return;
    }
    if (deleteTransaksiById(&app->transaksiList, t.id)) {
        msgOk("Transaksi ID %d (%s) dibatalkan.", t.id, t.namaPelanggan);
    } else {
        msgErr("Gagal hapus dari daftar transaksi.");
    }
}

static void menuAntrean(AppState *app) {
    while (1) {
        clearScreen();
        screenHeader("ANTREAN SERVIS & RIWAYAT", YEL);
        boxItem(YEL, 1, GRN, "Tambah Kendaraan ke Antrean");
        boxItem(YEL, 2, CYN, "Proses Servis (Dequeue)");
        boxItem(YEL, 3, MAG, "Lihat Antrean");
        boxItem(YEL, 4, BLU, "Lihat Riwayat (Stack)");
        boxItem(YEL, 5, RED, "Undo Transaksi Terakhir");
        boxItem(YEL, 0, WHT, "Kembali");
        boxBottom(YEL);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) break;
        clearScreen();
        switch (c) {
            case 1: aksiAntreanTambah(app);  break;
            case 2: aksiAntreanProses(app);  break;
            case 3: aksiAntreanLihat(app);   break;
            case 4: aksiAntreanRiwayat(app); break;
            case 5: aksiAntreanUndo(app);    break;
            default: msgErr("Pilihan tidak tersedia.");
        }
        pauseEnter();
    }
}

/* ----- Submenu 2: Katalog Layanan (BST) ----- */
static void aksiKatalogTambah(AppState *app) {
    int kode;
    char nama[MAX_STR];
    double harga;
    screenTitle("TAMBAH LAYANAN", GRN);
    if (!inputInt("Kode Layanan : ", &kode)) { msgErr("Kode tidak valid!"); return; }
    if (searchBST(app->katalogRoot, kode)) { msgErr("Kode sudah ada!"); return; }
    if (!inputTeks("Nama Layanan : ", nama, MAX_STR)) return;
    if (!inputDouble("Harga (Rp)   : ", &harga)) { msgErr("Harga tidak valid!"); return; }

    app->katalogRoot = insertBST(app->katalogRoot, kode, nama, harga);
    msgOk("Layanan '%s' ditambahkan.", nama);
}

static void aksiKatalogCari(const AppState *app) {
    int kode;
    screenTitle("CARI LAYANAN", CYN);
    if (!inputInt("Kode Layanan: ", &kode)) { msgErr("Kode tidak valid!"); return; }
    BSTNode *n = searchBST(app->katalogRoot, kode);
    if (n) {
        printKatalogHeader();
        printf("  " YEL "%-5d" RST " " WHT "%-30s" RST " " GRN "Rp%10.2f" RST "\n",
               n->kode, n->nama, n->harga);
        msgOk("Layanan ditemukan.");
    } else {
        msgErr("Layanan dengan kode %d tidak ditemukan.", kode);
    }
}

static void aksiKatalogHapus(AppState *app) {
    int kode;
    screenTitle("HAPUS LAYANAN", RED);
    if (!inputInt("Kode Layanan: ", &kode)) { msgErr("Kode tidak valid!"); return; }
    if (!searchBST(app->katalogRoot, kode)) { msgErr("Kode %d tidak ada.", kode); return; }
    app->katalogRoot = deleteBST(app->katalogRoot, kode);
    msgOk("Layanan kode %d dihapus.", kode);
}

static void aksiKatalogTraversal(const AppState *app, int mode) {
    if (mode == 4)      screenTitle("KATALOG - IN-ORDER (URUT KODE)", GRN);
    else if (mode == 5) screenTitle("KATALOG - PRE-ORDER", CYN);
    else                screenTitle("KATALOG - POST-ORDER", MAG);

    if (!app->katalogRoot) {
        msgInfo("Katalog masih kosong.");
        return;
    }
    printKatalogHeader();
    if (mode == 4)      inOrder(app->katalogRoot);
    else if (mode == 5) preOrder(app->katalogRoot);
    else                postOrder(app->katalogRoot);
}

static void menuKatalog(AppState *app) {
    while (1) {
        clearScreen();
        screenHeader("KATALOG LAYANAN (BST)", GRN);
        boxItem(GRN, 1, YEL, "Tambah Layanan");
        boxItem(GRN, 2, CYN, "Cari Layanan");
        boxItem(GRN, 3, RED, "Hapus Layanan");
        boxItem(GRN, 4, MAG, "Tampilkan In-Order");
        boxItem(GRN, 5, BLU, "Tampilkan Pre-Order");
        boxItem(GRN, 6, YEL, "Tampilkan Post-Order");
        boxItem(GRN, 0, WHT, "Kembali");
        boxBottom(GRN);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) break;
        clearScreen();
        switch (c) {
            case 1: aksiKatalogTambah(app); break;
            case 2: aksiKatalogCari(app);   break;
            case 3: aksiKatalogHapus(app);  break;
            case 4: case 5: case 6: aksiKatalogTraversal(app, c); break;
            default: msgErr("Pilihan tidak tersedia.");
        }
        pauseEnter();
    }
}

/* ----- Submenu 3: Database Member (Hash Table) ----- */
static void aksiMemberDaftar(AppState *app) {
    char plat[MAX_STR], nama[MAX_STR];
    screenTitle("DAFTARKAN MEMBER", GRN);
    if (!inputTeks("Plat Nomor   : ", plat, MAX_STR)) return;
    if (searchMember(&app->memberTable, plat)) {
        msgErr("Plat %s sudah terdaftar.", plat);
        return;
    }
    if (!inputTeks("Nama Pemilik : ", nama, MAX_STR)) return;
    if (insertMember(&app->memberTable, plat, nama, 0) == 1) {
        msgOk("Member %s (%s) terdaftar.", nama, plat);
    } else {
        msgErr("Gagal mendaftarkan member.");
    }
}

static void aksiMemberCari(AppState *app) {
    char plat[MAX_STR];
    screenTitle("CARI MEMBER", CYN);
    inputString("Plat Nomor: ", plat, MAX_STR);
    Member *m = searchMember(&app->memberTable, plat);
    if (m) {
        printf("\n  " CYN "%s" RST "  " WHT "%s" RST "  " GRN "%d poin" RST "\n",
               m->platNomor, m->nama, m->poin);
        msgOk("Member ditemukan.");
    } else {
        msgErr("Plat '%s' belum terdaftar.", plat);
    }
}

static void aksiMemberHapus(AppState *app) {
    char plat[MAX_STR];
    screenTitle("HAPUS MEMBER", RED);
    inputString("Plat Nomor: ", plat, MAX_STR);
    if (deleteMember(&app->memberTable, plat)) msgOk("Member %s dihapus.", plat);
    else                                       msgErr("Plat '%s' tidak ditemukan.", plat);
}

static void aksiMemberTampil(const AppState *app) {
    screenTitle("SEMUA MEMBER (HASH TABLE)", MAG);
    printHashTable(&app->memberTable);
}

static void menuMember(AppState *app) {
    while (1) {
        clearScreen();
        screenHeader("DATABASE MEMBER (HASH TABLE)", MAG);
        boxItem(MAG, 1, GRN, "Daftarkan Member");
        boxItem(MAG, 2, CYN, "Cari Member");
        boxItem(MAG, 3, RED, "Hapus Member");
        boxItem(MAG, 4, YEL, "Tampilkan Semua");
        boxItem(MAG, 0, WHT, "Kembali");
        boxBottom(MAG);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) break;
        clearScreen();
        switch (c) {
            case 1: aksiMemberDaftar(app); break;
            case 2: aksiMemberCari(app);   break;
            case 3: aksiMemberHapus(app);  break;
            case 4: aksiMemberTampil(app); break;
            default: msgErr("Pilihan tidak tersedia.");
        }
        pauseEnter();
    }
}

/* ----- Submenu 4: Peta Wilayah Panggilan (Graph) ----- */
static void aksiPetaTampil(const AppState *app) {
    screenTitle("PETA WILAYAH (ADJACENCY LIST)", CYN);
    if (app->petaGraph.count == 0) {
        msgInfo("Peta masih kosong.");
        return;
    }
    for (int i = 0; i < app->petaGraph.count; i++) {
        printf(" " YEL "%2d." RST " " BOLD "%s%-18s" RST " " MAG "→" RST " ",
               i, PALETTE[i % NPAL], app->petaGraph.vertices[i].nama);
        for (AdjNode *cur = app->petaGraph.vertices[i].head; cur; cur = cur->next) {
            printf(WHT "%s" RST CYN " | " RST, app->petaGraph.vertices[cur->dest].nama);
        }
        printf("\n");
    }
}

static void aksiPetaTraversal(const AppState *app, int mode) {
    char nama[MAX_STR];
    screenTitle(mode == 2 ? "BFS - TELUSURI MELEBAR" : "DFS - TELUSURI MENDALAM",
                mode == 2 ? GRN : MAG);
    if (app->petaGraph.count == 0) {
        msgInfo("Peta masih kosong.");
        return;
    }
    printf(" " YEL "Wilayah tersedia:" RST "\n");
    for (int i = 0; i < app->petaGraph.count; i++) {
        printf("   %s•%s %s\n", PALETTE[i % NPAL], RST, app->petaGraph.vertices[i].nama);
    }
    printf("\n");
    inputString("Nama Wilayah Awal: ", nama, MAX_STR);
    int idx = findVertex(&app->petaGraph, nama);
    if (idx < 0) {
        msgErr("Wilayah '%s' tidak ditemukan.", nama);
        return;
    }
    printf("\n ");
    if (mode == 2) BFS(&app->petaGraph, idx);
    else           DFS(&app->petaGraph, idx);
    msgOk("Penelusuran selesai.");
}

static void menuPeta(AppState *app) {
    while (1) {
        clearScreen();
        screenHeader("PETA LAYANAN PANGGILAN (GRAPH)", CYN);
        boxItem(CYN, 1, YEL, "Tampilkan Titik Wilayah");
        boxItem(CYN, 2, GRN, "BFS dari Titik Awal");
        boxItem(CYN, 3, MAG, "DFS dari Titik Awal");
        boxItem(CYN, 0, WHT, "Kembali");
        boxBottom(CYN);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) break;
        clearScreen();
        switch (c) {
            case 1: aksiPetaTampil(app);        break;
            case 2: case 3: aksiPetaTraversal(app, c); break;
            default: msgErr("Pilihan tidak tersedia.");
        }
        pauseEnter();
    }
}

/* ----- Submenu 5: Laporan & Analisis (Sorting & Searching) ----- */
static void tampilSemuaTransaksi(const AppState *app) {
    printTransaksiHeader();
    for (int i = 0; i < app->transaksiList.count; i++) {
        printTransaksi(&app->transaksiList.data[i]);
    }
}

static void aksiLaporanSort(AppState *app, int c) {
    SortKey key;
    int asc;
    const char *judul;

    if (c == 2)      { key = SORT_BY_ID;    asc = 1; judul = "SORT ID ASC (SELECTION SORT)"; }
    else if (c == 3) { key = SORT_BY_ID;    asc = 0; judul = "SORT ID DESC (SELECTION SORT)"; }
    else if (c == 4) { key = SORT_BY_BIAYA; asc = 1; judul = "SORT BIAYA ASC (INSERTION SORT)"; }
    else if (c == 5) { key = SORT_BY_BIAYA; asc = 0; judul = "SORT BIAYA DESC (INSERTION SORT)"; }
    else             { key = SORT_BY_NAMA;  asc = 1; judul = "SORT NAMA ASC (SELECTION SORT)"; }

    screenTitle(judul, YEL);
    if (c <= 3 || c == 6) {
        selectionSort(app->transaksiList.data, app->transaksiList.count, key, asc);
    } else {
        insertionSort(app->transaksiList.data, app->transaksiList.count, key, asc);
    }
    tampilSemuaTransaksi(app);
    msgOk("Data berhasil diurutkan.");
}

static void aksiLaporanSeq(const AppState *app) {
    char nama[MAX_STR];
    screenTitle("SEQUENTIAL SEARCH (NAMA)", CYN);
    inputString("Nama Pelanggan: ", nama, MAX_STR);
    int idx = sequentialSearchNama(app->transaksiList.data, app->transaksiList.count, nama);
    if (idx < 0) {
        msgErr("Nama '%s' tidak ditemukan.", nama);
        return;
    }
    printTransaksiHeader();
    for (int i = 0; i < app->transaksiList.count; i++) {
        if (strcmp(app->transaksiList.data[i].namaPelanggan, nama) == 0) {
            printTransaksi(&app->transaksiList.data[i]);
        }
    }
    msgOk("Ditemukan pertama di indeks %d.", idx);
}

static void aksiLaporanBin(AppState *app) {
    int id;
    screenTitle("BINARY SEARCH (ID)", MAG);
    if (!inputInt("ID Transaksi: ", &id)) {
        msgErr("ID tidak valid!");
        return;
    }
    selectionSort(app->transaksiList.data, app->transaksiList.count, SORT_BY_ID, 1);
    msgInfo("Data diurutkan ascending by ID dulu (syarat binary search).");
    int idx = binarySearchId(app->transaksiList.data, app->transaksiList.count, id);
    if (idx < 0) {
        msgErr("ID %d tidak ditemukan.", id);
        return;
    }
    printTransaksiHeader();
    printTransaksi(&app->transaksiList.data[idx]);
    msgOk("Ditemukan di indeks %d.", idx);
}

static void menuLaporan(AppState *app) {
    while (1) {
        clearScreen();
        screenHeader("LAPORAN & ANALISIS TRANSAKSI", BLU);
        boxItem(BLU, 1, GRN, "Tampilkan Semua Transaksi");
        boxItem(BLU, 2, YEL, "Sort ID      Asc   (Selection)");
        boxItem(BLU, 3, YEL, "Sort ID      Desc  (Selection)");
        boxItem(BLU, 4, MAG, "Sort Biaya   Asc   (Insertion)");
        boxItem(BLU, 5, MAG, "Sort Biaya   Desc  (Insertion)");
        boxItem(BLU, 6, CYN, "Sort Nama    Asc   (Selection)");
        boxItem(BLU, 7, RED, "Sequential Search (Nama)");
        boxItem(BLU, 8, GRN, "Binary Search (ID)");
        boxItem(BLU, 0, WHT, "Kembali");
        boxBottom(BLU);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) break;
        clearScreen();
        if (c >= 1 && c <= 8 && app->transaksiList.count == 0) {
            screenTitle("LAPORAN", BLU);
            msgInfo("Belum ada transaksi.");
        } else if (c == 1) {
            screenTitle("SEMUA TRANSAKSI", GRN);
            tampilSemuaTransaksi(app);
        } else if (c >= 2 && c <= 6) {
            aksiLaporanSort(app, c);
        } else if (c == 7) {
            aksiLaporanSeq(app);
        } else if (c == 8) {
            aksiLaporanBin(app);
        } else {
            msgErr("Pilihan tidak tersedia.");
        }
        pauseEnter();
    }
}

/* ----- Dashboard Utama ----- */
void menuUtama(AppState *app, const char *username) {
    char buf[BOX_W + 1];
    while (1) {
        double omzet = 0;
        for (int i = 0; i < app->transaksiList.count; i++) {
            omzet += app->transaksiList.data[i].biaya;
        }

        clearScreen();
        screenHeader("DASHBOARD BENGKEL TAMBAL BAN", MAG);
        snprintf(buf, sizeof(buf), "Halo, %s! Selamat bekerja.", username);
        boxText(MAG, BOLD YEL, buf);
        boxSep(MAG);
        snprintf(buf, sizeof(buf), "Antrean: %d   Transaksi: %d   Member: %d",
                 app->antreanServis.count, app->transaksiList.count, app->memberTable.count);
        boxText(MAG, CYN, buf);
        snprintf(buf, sizeof(buf), "Layanan: %d   Wilayah: %d   Omzet: Rp%.0f",
                 countBST(app->katalogRoot), app->petaGraph.count, omzet);
        boxText(MAG, GRN, buf);
        boxSep(MAG);
        boxItem(MAG, 1, GRN, "Antrean Servis & Riwayat (Queue & Stack)");
        boxItem(MAG, 2, YEL, "Katalog Layanan (BST)");
        boxItem(MAG, 3, MAG, "Database Member (Hash Table)");
        boxItem(MAG, 4, CYN, "Peta Wilayah Panggilan (Graph)");
        boxItem(MAG, 5, BLU, "Laporan & Analisis (Sorting/Searching)");
        boxItem(MAG, 0, RED, "Logout");
        boxBottom(MAG);
        confetti(BOX_W + 2);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) return;
        switch (c) {
            case 1: menuAntrean(app); break;
            case 2: menuKatalog(app); break;
            case 3: menuMember(app);  break;
            case 4: menuPeta(app);    break;
            case 5: menuLaporan(app); break;
            default: msgErr("Pilihan tidak tersedia."); pauseEnter();
        }
    }
}

/* ----- Menu Login & Register ----- */
static int aksiLogin(AppState *app, char *usernameOut, int size) {
    char u[MAX_STR], p[MAX_STR];
    screenTitle("LOGIN", GRN);
    inputString("Username : ", u, MAX_STR);
    inputPassword("Password : ", p, MAX_STR);
    if (loginAccount(&app->accounts, u, p)) {
        snprintf(usernameOut, (size_t)size, "%s", u);
        msgOk("Login berhasil. Selamat datang, %s!", u);
        loadingBar("Memuat dashboard...");
        return 1;
    }
    msgErr("Login gagal. Username/password salah.");
    return 0;
}

static void aksiRegister(AppState *app) {
    char u[MAX_STR], p[MAX_STR], p2[MAX_STR];
    screenTitle("REGISTER AKUN BARU", YEL);
    inputString("Username baru        : ", u, MAX_STR);
    inputPassword("Password             : ", p, MAX_STR);
    inputPassword("Ulangi Password      : ", p2, MAX_STR);
    if (strcmp(p, p2) != 0) {
        msgErr("Password tidak sama!");
        return;
    }
    int r = registerAccount(&app->accounts, u, p);
    if (r == 0) {
        saveAccounts(&app->accounts, getDataPath("akun.txt"));
        msgOk("Registrasi berhasil. Silakan login, %s!", u);
    } else if (r == -1) {
        msgErr("Username sudah digunakan.");
    } else if (r == -2) {
        msgErr("Username/password tidak boleh kosong atau berisi '|'.");
    } else {
        msgErr("Kapasitas user penuh.");
    }
}

int menuAuth(AppState *app, char *usernameOut, int size) {
    while (1) {
        clearScreen();
        drawLogo();
        boxTop(CYN);
        boxRainbow(CYN, "SELAMAT DATANG");
        boxSep(CYN);
        boxItem(CYN, 1, GRN, "Login");
        boxItem(CYN, 2, YEL, "Register");
        boxItem(CYN, 0, RED, "Keluar");
        boxBottom(CYN);

        int c;
        if (!inputInt("Pilih menu: ", &c)) {
            msgErr("Input tidak valid! Masukkan angka.");
            pauseEnter();
            continue;
        }
        if (c == 0) return 0;
        clearScreen();
        if (c == 1) {
            if (aksiLogin(app, usernameOut, size)) return 1;
        } else if (c == 2) {
            aksiRegister(app);
        } else {
            msgErr("Pilihan tidak tersedia.");
        }
        pauseEnter();
    }
}

void layarPerpisahan(void) {
    clearScreen();
    confetti(BOX_W + 2);
    boxTop(MAG);
    boxRainbow(MAG, "TERIMA KASIH!");
    boxRainbow(MAG, "Data sudah tersimpan. Sampai jumpa lagi!");
    boxBottom(MAG);
    confetti(BOX_W + 2);
    printf("\n");
}
