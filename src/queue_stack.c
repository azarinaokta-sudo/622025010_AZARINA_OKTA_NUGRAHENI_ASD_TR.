#include "queue_stack.h"
#include <stdio.h>

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

int isQueueEmpty(const Queue *q) {
    return q->count == 0;
}

int isQueueFull(const Queue *q) {
    return q->count >= MAX_QUEUE;
}

int enqueue(Queue *q, Transaksi t) {
    if (isQueueFull(q)) return 0;
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->items[q->rear] = t;
    q->count++;
    return 1;
}

int dequeue(Queue *q, Transaksi *out) {
    if (isQueueEmpty(q)) return 0;
    *out = q->items[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->count--;
    return 1;
}

void printQueue(const Queue *q) {
    if (isQueueEmpty(q)) {
        printf("  " YEL "(antrean kosong)" RST "\n");
        return;
    }
    int i = q->front;
    for (int k = 0; k < q->count; k++) {
        printf("  " MAG "[%d]" RST " ", k + 1);
        printTransaksi(&q->items[i]);
        i = (i + 1) % MAX_QUEUE;
    }
}

void initStack(Stack *s) {
    s->top = -1;
}

int isStackEmpty(const Stack *s) {
    return s->top == -1;
}

int isStackFull(const Stack *s) {
    return s->top >= MAX_STACK - 1;
}

int push(Stack *s, Transaksi t) {
    if (isStackFull(s)) return 0;
    s->items[++s->top] = t;
    return 1;
}

int pop(Stack *s, Transaksi *out) {
    if (isStackEmpty(s)) return 0;
    *out = s->items[s->top--];
    return 1;
}

void printStack(const Stack *s) {
    if (isStackEmpty(s)) {
        printf("  " YEL "(riwayat kosong)" RST "\n");
        return;
    }
    for (int i = s->top; i >= 0; i--) {
        printf("  " MAG "[%d]" RST " ", s->top - i + 1);
        printTransaksi(&s->items[i]);
    }
}
