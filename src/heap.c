#include "heap.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Compare two tasks: smaller priority first; tie-break by deadline lexicographically */
static int task_cmp(const Task *a, const Task *b)
{
    if (a->priority != b->priority)
        return (a->priority < b->priority) ? -1 : 1;
    int dl = strcmp(a->deadline, b->deadline);
    if (dl != 0)
        return (dl < 0) ? -1 : 1;
    return strcmp(a->desc, b->desc);
}

static void heap_reserve(Heap *h, int need)
{
    if (h->capacity >= need)
        return;
    int nc = h->capacity ? h->capacity : 8;
    while (nc < need)
        nc <<= 1;
    Task *na = (Task *)realloc(h->arr, (size_t)nc * sizeof(Task));
    if (!na)
    {
        perror("realloc");
        exit(1);
    }
    h->arr = na;
    h->capacity = nc;
}

void heap_init(Heap *h)
{
    h->arr = NULL;
    h->size = 0;
    h->capacity = 0;
}

void heap_free(Heap *h)
{
    free(h->arr);
    h->arr = NULL;
    h->size = h->capacity = 0;
}

int heap_is_empty(const Heap *h) { return h->size == 0; }

void heap_clear(Heap *h) { h->size = 0; }

static void swap(Task *a, Task *b)
{
    Task t = *a;
    *a = *b;
    *b = t;
}

static void bubble_up(Task *a, int i)
{
    while (i > 0)
    {
        int p = (i - 1) >> 1;
        if (task_cmp(&a[i], &a[p]) >= 0)
            break;
        swap(&a[i], &a[p]);
        i = p;
    }
}

static void bubble_down(Task *a, int n, int i)
{
    for (;;)
    {
        int l = (i << 1) + 1, r = l + 1, m = i;
        if (l < n && task_cmp(&a[l], &a[m]) < 0)
            m = l;
        if (r < n && task_cmp(&a[r], &a[m]) < 0)
            m = r;
        if (m == i)
            break;
        swap(&a[i], &a[m]);
        i = m;
    }
}

void heap_insert(Heap *h, const Task *t)
{
    heap_reserve(h, h->size + 1);
    h->arr[h->size] = *t;
    bubble_up(h->arr, h->size);
    h->size++;
}

int heap_peek_min(Heap *h, Task *out)
{
    if (h->size == 0)
        return 0;
    if (out)
        *out = h->arr[0];
    return 1;
}

int heap_extract_min(Heap *h, Task *out)
{
    if (h->size == 0)
        return 0;
    if (out)
        *out = h->arr[0];
    h->size--;
    if (h->size > 0)
    {
        h->arr[0] = h->arr[h->size];
        bubble_down(h->arr, h->size, 0);
    }
    return 1;
}

void heap_print_sorted(const Heap *h)
{
    if (h->size == 0)
    {
        puts("(empty)");
        return;
    }
    // Make a temp copy and extract in order
    Heap tmp;
    heap_init(&tmp);
    heap_reserve(&tmp, h->size);
    memcpy(tmp.arr, h->arr, (size_t)h->size * sizeof(Task));
    tmp.size = h->size;
    tmp.capacity = h->size;

    int idx = 1;
    Task t;
    while (heap_extract_min(&tmp, &t))
    {
        printf("%2d. [%d] %-12s  %s\n", idx++, t.priority, t.deadline, t.desc);
    }
    heap_free(&tmp);
}
