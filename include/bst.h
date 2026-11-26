#ifndef BST_H
#define BST_H

#include "config.h"

typedef struct BSTNode {
    int    kode;
    char   nama[MAX_STR];
    double harga;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *insertBST(BSTNode *root, int kode, const char *nama, double harga);
BSTNode *searchBST(BSTNode *root, int kode);
BSTNode *deleteBST(BSTNode *root, int kode);
void     inOrder(BSTNode *root);
void     preOrder(BSTNode *root);
void     postOrder(BSTNode *root);
int      countBST(const BSTNode *root);
void     freeBST(BSTNode *root);
int      loadKatalog(BSTNode **root, const char *filename);
int      saveKatalog(BSTNode *root, const char *filename);

#endif /* BST_H */
