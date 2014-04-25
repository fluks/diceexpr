#include "str.h"
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#define DEFAULT_STR_SIZE 10
#define SIZE_MULTIPLIER 2

static int
resize_str(str *s, size_t size);

str*
str_new(const char *chars) {
    str *s = malloc(sizeof(*s));
    if (s == NULL)
        return NULL;
    s->len = 0;
    s->s = NULL;

    if (chars != NULL && strlen(chars) + 1 > DEFAULT_STR_SIZE) 
        s->size = strlen(chars) + 1;
    else
        s->size = DEFAULT_STR_SIZE;
    if (resize_str(s, s->size) != 0) {
        free(s);
        return NULL;
    }
    s->s[0] = '\0';

    if (chars != NULL)
        strcpy(s->s, chars);

    return s;
}

void
str_free(str *s) {
    assert(s != NULL);

    free(s->s);
    free(s);
}

void
str_erase(str *s) {
    assert(s != NULL);

    s->len = 0;
    s->s[0] = '\0';
}

int
str_append_char(str *s, int c) {
    assert(s != NULL);

    if (s->len + 1 >= s->size) {
        s->size *= SIZE_MULTIPLIER;
        if (resize_str(s, s->size) != 0)
            return ENOMEM;
    }
    s->s[s->len++] = c;

    if (s->len + 1 >= s->size) {
        s->size *= SIZE_MULTIPLIER;
        if (resize_str(s, s->size) != 0)
            return ENOMEM;
    }
    s->s[s->len] = '\0';

    return 0;
}

int
str_append_chars(str *s, const char *chars) {
    assert(s != NULL);
    assert(chars != NULL);
    
    size_t len = strlen(chars);
    while (s->size < s->len + len + 1) {
        s->size *= SIZE_MULTIPLIER;
        if (resize_str(s, s->size) != 0)
            return ENOMEM;
    }

    strcpy(s->s + s->len, chars);
    s->len += len;

    return 0;
}

int
str_append_format(str *s, const char *format, ...) {
    assert(s != NULL);
    assert(format != NULL);

    int retval = 0;
    char *temp = NULL;
    va_list ap;
    va_start(ap, format);
    if (vasprintf(&temp, format, ap) == -1) {
        retval = -1;
        goto end;
    }
    str_append_chars(s, temp);
    free(temp);

    end:
        va_end(ap);

    return retval;
}

int
str_copy_to_chars(str *s, char **chars) {
    assert(s != NULL);
    assert(*chars == NULL);

    *chars = malloc(s->len + 1);
    if (*chars == NULL)
        return ENOMEM;
    strcpy(*chars, s->s);

    return 0;
}

static int
resize_str(str *s, size_t size) {
    assert(s != NULL);

    char *temp = realloc(s->s, size);
    if (temp == NULL)
        return ENOMEM;
    s->s = temp;

    return 0;
}
