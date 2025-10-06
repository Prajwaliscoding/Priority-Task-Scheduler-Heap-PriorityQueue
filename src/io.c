#include "heap.h"
#include "io.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

/* Save in ascending priority (stable enough for small sets) by using heap_print_sorted-like pass.
   For simplicity we write current array order; loading will heapify via inserts anyway. */

int heap_save_csv(const Heap *h, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
    {
        perror("fopen");
        return -1;
    }

    for (int i = 0; i < h->size; ++i)
    {
        // sanitize commas/newlines in desc/deadline by replacing newlines
        char desc[DESC_MAX], dl[DATE_MAX];
        safe_strcpy(desc, h->arr[i].desc, DESC_MAX);
        safe_strcpy(dl, h->arr[i].deadline, DATE_MAX);
        for (char *p = desc; *p; ++p)
            if (*p == '\n' || *p == '\r')
                *p = ' ';
        for (char *p = dl; *p; ++p)
            if (*p == '\n' || *p == '\r')
                *p = ' ';
        // NOTE: commas in desc are allowed but split logic on load is "first two commas".
        fprintf(f, "%s,%d,%s\n", desc, h->arr[i].priority, dl[0] ? dl : "-");
    }
    fclose(f);
    return 0;
}

int heap_load_csv(Heap *h, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
    {
        perror("fopen");
        return -1;
    }

    char line[DESC_MAX + DATE_MAX + 32];
    while (fgets(line, sizeof line, f))
    {
        trim_newline(line);

        // Split into three parts: desc,priority,deadline (deadline optional)
        char *first = strchr(line, ',');
        if (!first)
            continue;
        *first = '\0';
        char *second = strchr(first + 1, ','); // may be NULL
        char *s_desc = trim_spaces(line);
        char *s_prio = trim_spaces(first + 1);
        char *s_dead = "-";
        if (second)
        {
            *second = '\0';
            s_prio = trim_spaces(first + 1);
            s_dead = trim_spaces(second + 1);
            if (!*s_dead)
                s_dead = "-";
        }

        int pri;
        if (!parse_int(s_prio, &pri))
            continue;

        Task t;
        safe_strcpy(t.desc, s_desc, DESC_MAX);
        t.priority = pri;
        safe_strcpy(t.deadline, s_dead, DATE_MAX);

        heap_insert(h, &t);
    }

    fclose(f);
    return 0;
}
