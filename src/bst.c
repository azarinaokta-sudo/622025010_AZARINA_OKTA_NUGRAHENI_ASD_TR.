#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BSTNode *insertBST(BSTNode *root, int kode, const char *nama, double harga) {
    if (!root) {
        BSTNode *n = (BSTNode *)malloc(sizeof(BSTNode));
        if (!n) return NULL;
        n->kode = kode;
        snprintf(n->nama, MAX_STR, "%s", nama);
        n->harga = harga;
        n->left = NULL;
        n->right = NULL;
        return n;
    }
    if (kode < root->kode)      root->left  = insertBST(root->left,  kode, nama, harga);
    else if (kode > root->kode) root->right = insertBST(root->right, kode, nama, harga);
    return root;
}

BSTNode *searchBST(BSTNode *root, int kode) {
    if (!root) return NULL;
    if (root->kode == kode) return root;
    if (kode < root->kode)  return searchBST(root->left, kode);
    return searchBST(root->right, kode);
}

static BSTNode *minValueNode(BSTNode *node) {
    BSTNode *cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
}

BSTNode *deleteBST(BSTNode *root, int kode) {
    if (!root) return NULL;
    if (kode < root->kode)      root->left  = deleteBST(root->left,  kode);
    else if (kode > root->kode) root->right = deleteBST(root->right, kode);
    else {
        if (!root->left)  { BSTNode *t = root->right; free(root); return t; }
        if (!root->right) { BSTNode *t = root->left;  free(root); return t; }
        BSTNode *t = minValueNode(root->right);
        root->kode = t->kode;
        snprintf(root->nama, MAX_STR, "%s", t->nama);
        root->harga = t->harga;
        root->right = deleteBST(root->right, t->kode);
    }
    return root;
}

static void printNode(const BSTNode *n) {
    printf("  " YEL "%-5d" RST " " WHT "%-30s" RST " " GRN "Rp%10.2f" RST "\n",
           n->kode, n->nama, n->harga);
}

void inOrder(BSTNode *root) {
    if (!root) return;
    inOrder(root->left);
    printNode(root);
    inOrder(root->right);
}

void preOrder(BSTNode *root) {
    if (!root) return;
    printNode(root);
    preOrder(root->left);
    preOrder(root->right);
}

void postOrder(BSTNode *root) {
    if (!root) return;
    postOrder(root->left);
    postOrder(root->right);
    printNode(root);
}

int countBST(const BSTNode *root) {
    return root ? 1 + countBST(root->left) + countBST(root->right) : 0;
}

void freeBST(BSTNode *root) {
    if (!root) return;
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}

int loadKatalog(BSTNode **root, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        char *tok = strtok(line, "|"); if (!tok) continue;
        int kode = atoi(tok);
        tok = strtok(NULL, "|");       if (!tok) continue;
        char nama[MAX_STR]; snprintf(nama, MAX_STR, "%s", tok);
        tok = strtok(NULL, "|");       if (!tok) continue;
        double harga = atof(tok);

        *root = insertBST(*root, kode, nama, harga);
    }
    fclose(f);
    return 1;
}

static void saveBSTRec(FILE *f, const BSTNode *root) {
    if (!root) return;
    fprintf(f, "%d|%s|%.2f\n", root->kode, root->nama, root->harga);
    saveBSTRec(f, root->left);
    saveBSTRec(f, root->right);
}

int saveKatalog(BSTNode *root, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    saveBSTRec(f, root);
    fclose(f);
    return 1;
}
