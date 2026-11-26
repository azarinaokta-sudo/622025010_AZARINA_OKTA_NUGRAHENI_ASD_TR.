#include "auth.h"
#include <stdio.h>
#include <string.h>

void initAccounts(AccountList *list) {
    list->count = 0;
}

int loadAccounts(AccountList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    list->count = 0;
    char line[256];
    while (list->count < MAX_USER && fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        char *sep = strchr(line, '|');
        if (!sep) continue;
        *sep = '\0';
        snprintf(list->data[list->count].username, MAX_STR, "%.*s", MAX_STR - 1, line);
        snprintf(list->data[list->count].password, MAX_STR, "%.*s", MAX_STR - 1, sep + 1);
        list->count++;
    }
    fclose(f);
    return 1;
}

int saveAccounts(const AccountList *list, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;

    for (int i = 0; i < list->count; i++) {
        fprintf(f, "%s|%s\n", list->data[i].username, list->data[i].password);
    }
    fclose(f);
    return 1;
}

int findAccount(const AccountList *list, const char *username) {
    for (int i = 0; i < list->count; i++) {
        if (strcmp(list->data[i].username, username) == 0) return i;
    }
    return -1;
}

int registerAccount(AccountList *list, const char *username, const char *password) {
    if (strlen(username) == 0 || strlen(password) == 0) return -2;
    if (strchr(username, '|') || strchr(password, '|')) return -2;
    if (findAccount(list, username) >= 0) return -1;
    if (list->count >= MAX_USER) return -3;

    snprintf(list->data[list->count].username, MAX_STR, "%s", username);
    snprintf(list->data[list->count].password, MAX_STR, "%s", password);
    list->count++;
    return 0;
}

int loginAccount(const AccountList *list, const char *username, const char *password) {
    int idx = findAccount(list, username);
    if (idx < 0) return 0;
    return strcmp(list->data[idx].password, password) == 0;
}
