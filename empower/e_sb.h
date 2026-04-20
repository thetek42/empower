#ifndef E_SB_H_
#define E_SB_H_

/**************************************************************************************************
 *
 * This module implements string builders.
 *
 * String builders are dynamically allocated, resizable strings. The string is not null-terminated
 * (unless the user calls `e_sb_append_null()` before using the string).
 *
 * On allocation failure, an error message is printed and the programme is terminated.
 *
 * Configuration options:
 *  - `E_CONFIG_SB_SV_COMPAT`: When defined, enables compatibility with e_sv.h
 *
 **************************************************************************************************/

#include <stddef.h>

/**
 * String builder.
 *
 * The pointer `ptr` is never `NULL`.
 */
typedef struct {
    char *ptr;
    size_t len;
    size_t cap;
} E_Sb;

/**
 * Helper macro for formatting string builders in printf-style functions.
 *
 * This can be used together with E_SB_ARG as follows:
 *
 * ```
 * E_Sb hello = // ...
 * printf ("Message: %" E_SB_FMT "\n", E_SB_ARG (hello));
 * ```
 */
#define E_SB_FMT ".*s"

/**
 * Helper macro for formatting string views in printf-style functions.
 *
 * See E_SB_FMT.
 */
#define E_SB_ARG(sb) (int) (sb).len, (sb).ptr

E_Sb e_sb_init (void);
void e_sb_deinit (E_Sb *sb);
void e_sb_append_char (E_Sb *sb, char c);
void e_sb_append_buf (E_Sb *sb, const char *ptr, size_t len);
void e_sb_append (E_Sb *sb, const char *cstr);
void e_sb_append_null (E_Sb *sb);

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# if defined(__GNUC__) || defined(__clang__) || defined(__TINYC__)
size_t e_sb_append_fmt (E_Sb *sb, const char *fmt, ...) __attribute__ ((format (printf, 2, 3)));
# else  /* defined(__GNUC__) || defined(__clang__) || defined (__TINYC__) */
size_t e_sb_append_fmt (E_Sb *sb, const char *fmt, ...);
# endif /* defined(__GNUC__) || defined(__clang__) || defined (__TINYC__) */
#endif  /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

#ifdef E_CONFIG_SB_SV_COMPAT
# include "e_sv.h"
E_Sv e_sb_to_sv (const E_Sb *sb);
void e_sb_append_sv (E_Sb *sb, E_Sv sv);
#endif /* E_CONFIG_SB_SV_COMPAT */

/**************************************************************************************************/

#ifdef E_SB_IMPL

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define E_SB__INIT_CAP 128

void e_sb__reserve (E_Sb *sb, size_t cap);

/**
 * Initialise a new string builder.
 *
 * No memory is allocated yet.
 */
E_Sb
e_sb_init (void)
{
    E_Sb sb;
    sb.ptr = NULL;
    sb.len = 0;
    sb.cap = 0;
    return sb;
}

/**
 * Free the memory occupied by the string builder.
 */
void
e_sb_deinit (E_Sb *sb)
{
    free (sb->ptr);
}

/**
 * Append a single character to the string builder.
 */
void
e_sb_append_char (E_Sb *sb, char c)
{
    e_sb__reserve (sb, sb->len + 1);
    sb->ptr[sb->len] = c;
    sb->len += 1;
}

/**
 * Append multiple characters to the string builder.
 */
void
e_sb_append_buf (E_Sb *sb, const char *ptr, size_t len)
{
    e_sb__reserve (sb, sb->len + len);
    memcpy (&sb->ptr[sb->len], ptr, len);
    sb->len += len;
}

/**
 * Append a null-terminated string to the string builder.
 *
 * The length of the string is determined using strlen().
 */
void
e_sb_append (E_Sb *sb, const char *cstr)
{
    e_sb_append_buf (sb, cstr, strlen (cstr));
}

/**
 * Append a null byte (`\0`) to the string builder, without increasing its
 * length.
 *
 * This can be useful if you need to call an API that requires null-terminated
 * strings, while keeping the length intact (as it should, since the null
 * terminator is not really part of the string).
 *
 * If you append or remove things from the string builder in any way after
 * calling this function, you will no longer have a null-terminated string.
 */
void
e_sb_append_null (E_Sb *sb)
{
    e_sb__reserve (sb, sb->len + 1);
    sb->ptr[sb->len] = 0;
}

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
/**
 * Append a printf-style formatted string to a string builder.
 */
size_t
e_sb_append_fmt (E_Sb *sb, const char *fmt, ...)
{
    va_list ap;
    size_t n;

    va_start (ap, fmt);
    n = (size_t) vsnprintf (NULL, 0, fmt, ap); /* NOLINT */
    va_end (ap);

    e_sb__reserve (sb, sb->len + n + 1);
    va_start (ap, fmt);
    vsnprintf (&sb->ptr[sb->len], n + 1, fmt, ap); /* NOLINT */
    va_end (ap);
    sb->len += n;
    return n;
}
# endif /* defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L */

# ifdef E_CONFIG_SB_SV_COMPAT

E_Sv
e_sb_to_sv (const E_Sb *sb)
{
    E_Sv sv;
    sv.ptr = sb->ptr;
    sv.len = sb->len;
    return sv;
}

void
e_sb_append_sv (E_Sb *sb, E_Sv sv)
{
    e_sb_append_buf (sb, sv.ptr, sv.len);
}

# endif /* E_CONFIG_SB_SV_COMPAT */

void
e_sb__reserve (E_Sb *sb, size_t cap)
{
    char *ptr;
    if (cap <= sb->cap) return;
    if (sb->cap == 0) sb->cap = E_SB__INIT_CAP;
    while (sb->cap < cap)
        sb->cap *= 2;
    ptr = realloc (sb->ptr, sb->cap * sizeof (char));
    if (ptr == NULL) {
        fprintf (stderr, "[e_sb] allocation failed\n");
        abort ();
    }
    sb->ptr = ptr;
}

#endif /* E_SB_IMPL */

#endif /* E_SB_H_ */
