#ifndef IO_H
#define IO_H

#include "heap.h"

/* CSV persistence: "desc,priority,deadline" (desc/newlines sanitized) */
int heap_save_csv(const Heap *h, const char *path);
int heap_load_csv(Heap *h, const char *path); // merges into existing heap

#endif /* IO_H */
