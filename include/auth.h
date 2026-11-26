#ifndef AUTH_H
#define AUTH_H

#include "config.h"

typedef struct {
    char username[MAX_STR];
    char password[MAX_STR];
} Account;

typedef struct {
    Account data[MAX_USER];
    int count;
} AccountList;

void initAccounts(AccountList *list);
int  loadAccounts(AccountList *list, const char *filename);
int  saveAccounts(const AccountList *list, const char *filename);
int  findAccount(const AccountList *list, const char *username);

/* return: 0 sukses, -1 duplikat, -2 kosong/tidak valid, -3 penuh */
int  registerAccount(AccountList *list, const char *username, const char *password);
int  loginAccount(const AccountList *list, const char *username, const char *password);

#endif /* AUTH_H */
