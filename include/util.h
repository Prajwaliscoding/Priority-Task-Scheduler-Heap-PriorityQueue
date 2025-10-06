#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/* strings */
void safe_strcpy(char *dst, const char *src, size_t cap);
char *trim_spaces(char *s);
void trim_newline(char *s);

/* parsing */
int parse_int(const char *s, int *out);

/* command helpers */
int parse_add_command(const char *line, char *out_desc, int *out_pri, char *out_deadline,
                      size_t desc_cap, size_t dl_cap);
/* Expects: add "Some desc with spaces" 2 2025-12-31
   Returns 1 on success, 0 on failure. */

#endif /* UTIL_H */
