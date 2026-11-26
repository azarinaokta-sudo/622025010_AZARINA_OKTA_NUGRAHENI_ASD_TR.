#ifndef QUEUE_STACK_H
#define QUEUE_STACK_H

#include "config.h"
#include "transaksi.h"

/* Queue (FIFO) untuk antrean kendaraan servis */
typedef struct {
    Transaksi items[MAX_QUEUE];
    int front;
    int rear;
    int count;
} Queue;

/* Stack (LIFO) untuk riwayat & undo transaksi */
typedef struct {
    Transaksi items[MAX_STACK];
    int top;
} Stack;

void initQueue(Queue *q);
int  isQueueEmpty(const Queue *q);
int  isQueueFull(const Queue *q);
int  enqueue(Queue *q, Transaksi t);
int  dequeue(Queue *q, Transaksi *out);
void printQueue(const Queue *q);

void initStack(Stack *s);
int  isStackEmpty(const Stack *s);
int  isStackFull(const Stack *s);
int  push(Stack *s, Transaksi t);
int  pop(Stack *s, Transaksi *out);
void printStack(const Stack *s);

#endif /* QUEUE_STACK_H */
