# Sistem Manajemen Operasional & Layanan Tambal Ban Panggilan

Tugas Rancang (TR) Praktikum **Algoritma dan Struktur Data (ASD) 2026** — Perangkat lunak berbasis konsol yang ditulis 100% dalam Bahasa C dengan arsitektur modular (`.h` dan `.c`).

---

## Identitas Mahasiswa

- **NIM**        : `[Isi NIM Kamu]`
- **Nama**       : `[Isi Nama Lengkap Kamu]`
- **Mata Kuliah**: Praktikum Algoritma dan Struktur Data (ASD) 2026
- **Tema**       : Sistem Manajemen Operasional & Layanan Tambal Ban Panggilan

---

## Deskripsi Sistem & Penerapan Materi (Topik 1 – 8)

Aplikasi ini membantu operasional bengkel tambal ban panggilan mulai dari autentikasi petugas, pengelolaan antrean kendaraan, pencatatan transaksi, manajemen member, katalog layanan, pemetaan wilayah panggilan darurat, hingga laporan penjualan.

1. **Login, Register & Logout (`auth.h`, `auth.c`)**
   - Autentikasi petugas sebelum masuk ke dashboard utama, validasi username unik, pengecekan input kosong/tidak valid, dan penyimpanan permanen pada `data/akun.txt`.
2. **Topik 1 — Refreshing C (`ui.h`, `ui.c`, `menu.h`, `menu.c`)**
   - Program interaktif berbasis menu bersarang menggunakan perulangan, percabangan, fungsi modular satu tanggung jawab, serta validasi input (`fgets`, `strtol`, `strtod`) agar bebas dari *crash* maupun *infinite loop*.
3. **Topik 2 — Array, Pointer & File (`transaksi.h`, `transaksi.c`)**
   - Data transaksi dikelola menggunakan `struct` dan *dynamic array* (`realloc`/`free`), *pass-by-reference* menggunakan pointer, serta persistensi baca/tulis pada berkas `data/*.txt`.
4. **Topik 3 — Sorting (`sorting_searching.h`, `sorting_searching.c`)**
   - Implementasi mandiri **Selection Sort** dan **Insertion Sort** (tanpa `qsort`) dengan opsi pengurutan **Ascending** dan **Descending** berdasarkan ID, Biaya, maupun Nama Pelanggan.
5. **Topik 4 — Stack & Queue (`queue_stack.h`, `queue_stack.c`)**
   - **Queue (FIFO)** untuk antrean kendaraan yang menunggu servis, dan **Stack (LIFO)** untuk riwayat servis selesai sekaligus fitur **Undo** transaksi terakhir.
6. **Topik 5 — Searching (`sorting_searching.h`, `sorting_searching.c`)**
   - Implementasi mandiri **Sequential Search** (pencarian nama pelanggan) dan **Binary Search** (pencarian ID transaksi setelah data diurutkan ascending).
7. **Topik 6 — Hash Table (`hashtable.h`, `hashtable.c`)**
   - Database pelanggan member menggunakan fungsi hash **djb2** dengan kunci unik **Plat Nomor** dan penanganan *collision* menggunakan metode **Chaining** (*linked list*).
8. **Topik 7 — Binary Search Tree (`bst.h`, `bst.c`)**
   - Katalog layanan bengkel disimpan dalam **BST** berdasarkan kode layanan, mendukung operasi *Insert*, *Search*, *Delete*, serta traversal **In-Order**, **Pre-Order**, dan **Post-Order**.
9. **Topik 8 — Graph (`graph.h`, `graph.c`)**
   - Peta jangkauan wilayah layanan panggilan direpresentasikan menggunakan **Adjacency List** tak berarah, dilengkapi penelusuran **BFS** (*Breadth-First Search*) dan **DFS** (*Depth-First Search*).

---

## Struktur Folder

```text
NIM_NAMA_ASD_TR/
├── include/                    # Berkas header (.h)
│   ├── auth.h
│   ├── bst.h
│   ├── config.h
│   ├── graph.h
│   ├── hashtable.h
│   ├── menu.h
│   ├── queue_stack.h
│   ├── sorting_searching.h
│   ├── transaksi.h
│   └── ui.h
├── src/                        # Berkas sumber (.c)
│   ├── auth.c
│   ├── bst.c
│   ├── graph.c
│   ├── hashtable.c
│   ├── main.c
│   ├── menu.c
│   ├── queue_stack.c
│   ├── sorting_searching.c
│   ├── transaksi.c
│   └── ui.c
├── data/                       # Database permanen (.txt)
│   ├── akun.txt
│   ├── katalog.txt
│   ├── member.txt
│   ├── peta.txt
│   └── transaksi.txt
├── docs/                       # Laporan TR (PDF)
├── .gitignore                  # Mengabaikan berkas hasil kompilasi (*.exe, *.o)
└── README.md                   # Dokumentasi proyek & panduan kompilasi
```

---

## Cara Kompilasi dan Menjalankan Program

Pastikan kompiler **GCC** (MinGW-w64 pada Windows atau GCC pada Linux/macOS) sudah terpasang. Buka terminal pada folder utama proyek (`NIM_NAMA_ASD_TR/`), lalu jalankan perintah berikut:

### Windows (CMD / PowerShell / Terminal VS Code)
```bash
gcc -std=c11 -Wall -Wextra -Iinclude src/*.c -o app.exe
app.exe
```

### Linux / macOS
```bash
gcc -std=c11 -Wall -Wextra -Iinclude src/*.c -o app
./app
```

> **Catatan:** Sebelum melakukan `git push` ke GitHub atau membuat arsip `.zip`/`.rar`, pastikan berkas hasil kompilasi (`app.exe` / `*.o`) tidak ikut terunggah agar *language bar* repositori tetap **100% C**.
