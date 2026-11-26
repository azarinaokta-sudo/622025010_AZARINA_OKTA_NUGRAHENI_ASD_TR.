#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
#endif

const char *PALETTE[NPAL] = { RED, YEL, GRN, CYN, BLU, MAG };

void msleep(int ms) {
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

void initTerminal(void) {
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    SetConsoleOutputCP(65001);
    if (GetConsoleMode(h, &mode)) {
        SetConsoleMode(h, mode | 0x0004);
    }
#endif
    srand((unsigned)time(NULL));
}

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[3J\033[H");
    fflush(stdout);
#endif
}

void rainbow(const char *s, int offset) {
    int k = offset;
    for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
        if ((*p & 0xC0) != 0x80) {
            printf("%s", PALETTE[k % NPAL]);
            k++;
        }
        putchar(*p);
    }
    printf(RST);
}

void repeatStr(const char *s, int n) {
    while (n-- > 0) fputs(s, stdout);
}

void confetti(int n) {
    static const char *sym[] = { "★", "✦", "♦", "•", "*", "+", "✧", "°" };
    for (int i = 0; i < n; i++) {
        if (rand() % 3 == 0) {
            printf("%s%s" RST, PALETTE[rand() % NPAL], sym[rand() % 8]);
        } else {
            putchar(' ');
        }
    }
    printf("\n");
}

void boxTop(const char *c)    { printf("%s╔", c); repeatStr("═", BOX_W); printf("╗" RST "\n"); }
void boxSep(const char *c)    { printf("%s╠", c); repeatStr("═", BOX_W); printf("╣" RST "\n"); }
void boxBottom(const char *c) { printf("%s╚", c); repeatStr("═", BOX_W); printf("╝" RST "\n"); }

void boxRainbow(const char *c, const char *text) {
    int len = (int)strlen(text);
    int left = (BOX_W - len) / 2;
    int right = BOX_W - len - left;
    if (left < 0) left = 0;
    if (right < 0) right = 0;

    printf("%s║%s%*s", c, RST, left, "");
    printf(BOLD);
    rainbow(text, rand() % NPAL);
    printf("%*s%s║%s\n", right, "", c, RST);
}

void boxText(const char *c, const char *tc, const char *text) {
    int pad = BOX_W - 1 - (int)strlen(text);
    if (pad < 0) pad = 0;
    printf("%s║%s %s%s%s%*s%s║%s\n", c, RST, tc, text, RST, pad, "", c, RST);
}

void boxItem(const char *c, int num, const char *nc, const char *text) {
    int pad = BOX_W - (5 + (int)strlen(text));
    if (pad < 0) pad = 0;
    printf("%s║%s %s%s[%d]%s %s%s%s%*s%s║%s\n",
           c, RST, BOLD, nc, num, RST, WHT, text, RST, pad, "", c, RST);
}

void screenHeader(const char *title, const char *c) {
    confetti(BOX_W + 2);
    boxTop(c);
    boxRainbow(c, title);
    boxSep(c);
}

void screenTitle(const char *title, const char *c) {
    confetti(BOX_W + 2);
    boxTop(c);
    boxRainbow(c, title);
    boxBottom(c);
    printf("\n");
}

void drawLogo(void) {
    static const char *tire[] = {
        "      _.-'''''-._      ",
        "    .'  .-'''-.  '.    ",
        "   /   /  .-.  \\   \\   ",
        "  |   |  ( O )  |   |  ",
        "   \\   \\  '-'  /   /   ",
        "    '.  '-...-'  .'    ",
        "      '-._____.-'      "
    };
    confetti(BOX_W + 2);
    for (int i = 0; i < 7; i++) {
        printf("%*s%s%s%s" RST "\n", 19, "", BOLD, PALETTE[(i + rand()) % NPAL], tire[i]);
    }
    printf("\n%*s", 15, "");
    rainbow("★ SISTEM TAMBAL BAN PANGGILAN ★", 0);
    printf("\n%*s" WHT "Cepat • Murah • Terpercaya" RST "\n", 18, "");
    confetti(BOX_W + 2);
    printf("\n");
}

void loadingBar(const char *label) {
    const int n = 40;
    printf("\n " BOLD WHT "%s" RST "\n", label);
    for (int i = 0; i <= n; i++) {
        printf("\r " MAG "[" RST);
        for (int j = 0; j < n; j++) {
            if (j < i) printf("%s█", PALETTE[j % NPAL]);
            else       printf(RST "░");
        }
        printf(RST MAG "]" RST " %3d%%", i * 100 / n);
        fflush(stdout);
        msleep(20);
    }
    printf("\n");
}

void msgOk(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("\n " GRN BOLD "✔ " RST GRN);
    vprintf(fmt, ap);
    printf(RST "\n");
    va_end(ap);
}

void msgErr(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("\n " RED BOLD "✘ " RST RED);
    vprintf(fmt, ap);
    printf(RST "\n");
    va_end(ap);
}

void msgInfo(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("\n " YEL BOLD "➜ " RST YEL);
    vprintf(fmt, ap);
    printf(RST "\n");
    va_end(ap);
}

void pauseEnter(void) {
    int c;
    printf("\n " MAG BOLD "» Tekan ENTER untuk lanjut..." RST);
    fflush(stdout);
    while ((c = getchar()) != '\n' && c != EOF) { }
}

static void flushLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void inputString(const char *prompt, char *buf, int size) {
    printf(" " CYN BOLD "➤ %s" RST YEL BOLD, prompt);
    fflush(stdout);
    if (fgets(buf, size, stdin)) {
        if (strchr(buf, '\n') == NULL) flushLine();
        buf[strcspn(buf, "\r\n")] = 0;
    } else {
        buf[0] = 0;
    }
    printf(RST);
}

void inputPassword(const char *prompt, char *buf, int size) {
#ifdef _WIN32
    int i = 0, ch;
    printf(" " CYN BOLD "➤ %s" RST YEL BOLD, prompt);
    fflush(stdout);
    while ((ch = _getch()) != '\r' && ch != '\n') {
        if (ch == 8 || ch == 127) {
            if (i > 0) { i--; printf("\b \b"); }
        } else if (ch == 0 || ch == 224) {
            _getch();
        } else if (ch >= 32 && i < size - 1) {
            buf[i++] = (char)ch;
            putchar('*');
        }
    }
    buf[i] = 0;
    printf(RST "\n");
#else
    inputString(prompt, buf, size);
#endif
}

int inputTeks(const char *prompt, char *buf, int size) {
    inputString(prompt, buf, size);
    if (buf[0] == '\0') {
        msgErr("Input tidak boleh kosong!");
        return 0;
    }
    if (strchr(buf, '|')) {
        msgErr("Karakter '|' tidak boleh dipakai!");
        return 0;
    }
    return 1;
}

int inputInt(const char *prompt, int *out) {
    char buf[64];
    printf(" " CYN BOLD "➤ %s" RST YEL BOLD, prompt);
    fflush(stdout);
    if (!fgets(buf, sizeof(buf), stdin)) {
        printf(RST);
        return 0;
    }
    printf(RST);
    if (strchr(buf, '\n') == NULL) flushLine();

    char *end;
    long val = strtol(buf, &end, 10);
    if (end == buf) return 0;
    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') end++;
    if (*end != '\0') return 0;
    *out = (int)val;
    return 1;
}

int inputDouble(const char *prompt, double *out) {
    char buf[64];
    printf(" " CYN BOLD "➤ %s" RST YEL BOLD, prompt);
    fflush(stdout);
    if (!fgets(buf, sizeof(buf), stdin)) {
        printf(RST);
        return 0;
    }
    printf(RST);
    if (strchr(buf, '\n') == NULL) flushLine();

    char *end;
    double val = strtod(buf, &end);
    if (end == buf) return 0;
    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') end++;
    if (*end != '\0') return 0;
    if (val < 0) return 0;
    *out = val;
    return 1;
}

void getToday(char *buf, int size) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (tm) strftime(buf, (size_t)size, "%Y-%m-%d", tm);
    else    snprintf(buf, (size_t)size, "0000-00-00");
}

void printTransaksiHeader(void) {
    printf("\n" BOLD CYN "%-5s %-22s %-12s %-22s %12s  %s" RST "\n",
           "ID", "Nama", "Plat", "Kerusakan", "Biaya", "Tanggal");
    printf(MAG);
    repeatStr("═", 89);
    printf(RST "\n");
}

void printKatalogHeader(void) {
    printf("\n" BOLD CYN "  %-5s %-30s %12s" RST "\n", "KODE", "LAYANAN", "HARGA");
    printf(MAG);
    repeatStr("═", 51);
    printf(RST "\n");
}
