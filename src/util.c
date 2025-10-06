#include "util.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void safe_strcpy(char *dst, const char *src, size_t cap)
{
    if (!dst || cap == 0)
        return;
    if (!src)
    {
        dst[0] = '\0';
        return;
    }
    strncpy(dst, src, cap - 1);
    dst[cap - 1] = '\0';
}

char *trim_spaces(char *s)
{
    if (!s)
        return s;
    while (*s && isspace((unsigned char)*s))
        s++;
    size_t n = strlen(s);
    while (n && isspace((unsigned char)s[n - 1]))
        s[--n] = '\0';
    return s;
}

void trim_newline(char *s)
{
    if (!s)
        return;
    size_t n = strlen(s);
    while (n && (s[n - 1] == '\n' || s[n - 1] == '\r'))
        s[--n] = '\0';
}

int parse_int(const char *s, int *out)
{
    if (!s || !*s)
        return 0;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || *trim_spaces(end) != '\0')
        return 0;
    *out = (int)v;
    return 1;
}

int parse_add_command(const char *line, char *out_desc, int *out_pri, char *out_deadline,
                      size_t desc_cap, size_t dl_cap)
{
    // Expect line starts with "add "
    const char *p = line;
    while (*p && !isspace((unsigned char)*p))
        p++; // skip "add"
    while (*p && isspace((unsigned char)*p))
        p++;

    // Must start with a quote
    if (*p != '"')
        return 0;
    p++; // after opening quote

    // capture description until closing quote
    const char *start = p;
    while (*p && *p != '"')
        p++;
    if (*p != '"')
        return 0; // missing closing quote
    size_t len = (size_t)(p - start);
    if (len >= desc_cap)
        len = desc_cap - 1;
    memcpy(out_desc, start, len);
    out_desc[len] = '\0';

    p++; // after closing quote
    // skip whitespace
    while (*p && isspace((unsigned char)*p))
        p++;

    // parse priority
    const char *pri_start = p;
    while (*p && !isspace((unsigned char)*p))
        p++;
    char pri_buf[32];
    size_t plen = (size_t)(p - pri_start);
    if (plen == 0 || plen >= sizeof(pri_buf))
        return 0;
    memcpy(pri_buf, pri_start, plen);
    pri_buf[plen] = '\0';
    if (!parse_int(pri_buf, out_pri))
        return 0;

    // skip whitespace to deadline
    while (*p && isspace((unsigned char)*p))
        p++;

    // read deadline token to end (optional; if missing, set "-")
    if (!*p)
    {
        safe_strcpy(out_deadline, "-", dl_cap);
        return 1;
    }
    const char *dl_start = p;
    while (*p && !isspace((unsigned char)*p))
        p++;
    size_t dlen = (size_t)(p - dl_start);
    if (dlen == 0)
    {
        safe_strcpy(out_deadline, "-", dl_cap);
    }
    else
    {
        if (dlen >= dl_cap)
            dlen = dl_cap - 1;
        memcpy(out_deadline, dl_start, dlen);
        out_deadline[dlen] = '\0';
    }
    return 1;
}
