#include "heap.h"
#include "io.h"
#include "util.h"
#include <stdio.h>
#include <string.h>

#define LINE_MAX (DESC_MAX + DATE_MAX + 128)

static void help(void)
{
    puts(
        "Priority Task Scheduler (C)\n"
        "Commands:\n"
        "  add \"<desc>\" <priority> <deadline>\n"
        "     - Example: add \"Fix login bug\" 1 2025-10-10\n"
        "       (Deadline optional; use '-' if unknown)\n"
        "  pop                - Complete and remove the highest-priority task\n"
        "  peek               - Show the next task without removing\n"
        "  list               - List all pending tasks (sorted)\n"
        "  save <file.csv>    - Save tasks to CSV\n"
        "  load <file.csv>    - Load tasks from CSV (merge)\n"
        "  clear              - Remove all tasks\n"
        "  help               - Show this help\n"
        "  quit               - Exit\n");
}

int main(void)
{
    Heap h;
    heap_init(&h);

    puts("Priority Task Scheduler — type 'help' for commands.");
    char line[LINE_MAX];

    for (;;)
    {
        printf("> ");
        if (!fgets(line, sizeof line, stdin))
            break;
        trim_newline(line);
        char *p = trim_spaces(line);
        if (!*p)
            continue;

        if (strncmp(p, "add ", 4) == 0)
        {
            char desc[DESC_MAX], deadline[DATE_MAX];
            int pri;
            if (!parse_add_command(p, desc, &pri, deadline, sizeof desc, sizeof deadline))
            {
                puts("Usage: add \"<desc>\" <priority> <deadline>");
                continue;
            }
            Task t;
            safe_strcpy(t.desc, desc, DESC_MAX);
            t.priority = pri;
            safe_strcpy(t.deadline, deadline, DATE_MAX);
            heap_insert(&h, &t);
            printf("Added: \"%s\" (priority %d, deadline %s)\n", t.desc, t.priority, t.deadline);
        }
        else if (strcmp(p, "pop") == 0)
        {
            Task t;
            if (heap_extract_min(&h, &t))
            {
                printf("Completed: \"%s\" (priority %d, deadline %s)\n", t.desc, t.priority, t.deadline);
            }
            else
            {
                puts("(no tasks)");
            }
        }
        else if (strcmp(p, "peek") == 0)
        {
            Task t;
            if (heap_peek_min(&h, &t))
            {
                printf("Next: [%d] %-12s  %s\n", t.priority, t.deadline, t.desc);
            }
            else
            {
                puts("(no tasks)");
            }
        }
        else if (strcmp(p, "list") == 0)
        {
            heap_print_sorted(&h);
        }
        else if (strncmp(p, "save ", 5) == 0)
        {
            char *path = trim_spaces(p + 5);
            if (!*path)
            {
                puts("Usage: save <file.csv>");
                continue;
            }
            if (heap_save_csv(&h, path) == 0)
                printf("Saved tasks to %s\n", path);
        }
        else if (strncmp(p, "load ", 5) == 0)
        {
            char *path = trim_spaces(p + 5);
            if (!*path)
            {
                puts("Usage: load <file.csv>");
                continue;
            }
            if (heap_load_csv(&h, path) == 0)
                printf("Loaded tasks from %s\n", path);
        }
        else if (strcmp(p, "clear") == 0)
        {
            heap_clear(&h);
            puts("Cleared all tasks.");
        }
        else if (strcmp(p, "help") == 0)
        {
            help();
        }
        else if (strcmp(p, "quit") == 0 || strcmp(p, "exit") == 0)
        {
            break;
        }
        else
        {
            puts("Unknown command. Type 'help'.");
        }
    }

    heap_free(&h);
    return 0;
}
