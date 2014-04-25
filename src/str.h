#ifndef STR_H
    #define STR_H
#include <stddef.h>

/** A string library.
 * The data of the strings are handled as bytes, so the length of a string may not
 * be the same as the number of characters in it.
 */

/** String struct.
 * Will be null terminated at all times.
 */
typedef struct str {
    char *s;
    // Length of current string without '\0'.
    size_t len;
    // Amount of memory allocated.
    size_t size;
};

/** Create new str on the heap.
 * @param chars An initial string to copy to str, can be NULL.
 * @return New str or NULL if can't allocate memory.
 */
str*
str_new(const char *chars);

/** Free str type and its data.
 * @param s Can't be NULL.
 * @return void
 */
void
str_free(str *s);

/** Zero out all data.
 * @param s Can't be NULL.
 * @return void
 */
void
str_erase(str *s);

/** Append a character.
 * @param s Can't be NULL.
 * @param c
 * @return Zero on success, ENOMEM on error.
 */
int
str_append_char(str *s, int c);

/** Append a string.
 * @param s Can't be NULL.
 * @param chars String to append, can't be NULL.
 * @return Zero on success, ENOMEM on error.
 */
int
str_append_chars(str *s, const char *chars);

/** Copy str's data to a string.
 * chars will s->len + 1 of size.
 * @param s Can't be NULL.
 * @param chars String to copy to, must point to NULL.
 * @return Zero on success, ENOMEM on error.
 */
int
str_copy_to_chars(str *s, char **chars);

#endif // STR_H
