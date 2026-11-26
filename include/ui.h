#ifndef UI_H
#define UI_H

#include "config.h"

extern const char *PALETTE[NPAL];

/* Utilitas terminal & tampilan */
void initTerminal(void);
void clearScreen(void);
void msleep(int ms);
void rainbow(const char *s, int offset);
void repeatStr(const char *s, int n);
void confetti(int n);

/* Komponen kotak & layar */
void boxTop(const char *c);
void boxSep(const char *c);
void boxBottom(const char *c);
void boxRainbow(const char *c, const char *text);
void boxText(const char *c, const char *tc, const char *text);
void boxItem(const char *c, int num, const char *nc, const char *text);
void screenHeader(const char *title, const char *c);
void screenTitle(const char *title, const char *c);
void drawLogo(void);
void loadingBar(const char *label);

/* Pesan status */
void msgOk(const char *fmt, ...);
void msgErr(const char *fmt, ...);
void msgInfo(const char *fmt, ...);
void pauseEnter(void);

/* Validasi input aman dari infinite loop */
void inputString(const char *prompt, char *buf, int size);
void inputPassword(const char *prompt, char *buf, int size);
int  inputTeks(const char *prompt, char *buf, int size);
int  inputInt(const char *prompt, int *out);
int  inputDouble(const char *prompt, double *out);

/* Helper tanggal & header tabel */
void getToday(char *buf, int size);
void printTransaksiHeader(void);
void printKatalogHeader(void);

#endif /* UI_H */
