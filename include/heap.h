#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

#define DESC_MAX 128
#define DATE_MAX 32

typedef struct
{
    char desc[DESC_MAX];     // short task description
    int priority;            // smaller = higher priority
    char deadline[DATE_MAX]; // free-form: YYYY-MM-DD (recommended)
} Task;

typedef struct
{
    Task *arr;
    int size;
    int capacity;
} Heap;

/* Initialize / destroy */
void heap_init(Heap *h);
void heap_free(Heap *h);

/* Core ops */
void heap_insert(Heap *h, const Task *t); // O(log n)
int heap_extract_min(Heap *h, Task *out); // O(log n), 1 on success, 0 if empty
int heap_peek_min(Heap *h, Task *out);    // O(1), 1 on success, 0 if empty

/* Utilities */
int heap_is_empty(const Heap *h);
void heap_clear(Heap *h);

/* Debug / display helpers */
void heap_print_sorted(const Heap *h); // prints tasks in priority order (uses a temp copy)

#endif /* HEAP_H */
