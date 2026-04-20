#ifndef E_SV_H_
#define E_SV_H_

/**************************************************************************************************
 *
 * This module implements string views.
 *
 * String views are string slices that consist of a pointer to the string and
 * an associated length. The string is not null-terminated.
 *
 **************************************************************************************************/

#include <stddef.h>

/**
 * String view.
 *
 * The pointer `ptr` is never `NULL`.
 */
typedef struct {
    const char *ptr;
    size_t len;
} E_Sv;

/**
 * Helper macro for formatting string views in printf-style functions.
 *
 * This can be used together with E_SV_ARG as follows:
 *
 * ```
 * E_Sv hello = e_sv_from_cstr ("Hello, World!");
 * printf ("Message: %" E_SV_FMT "\n", E_SV_ARG (&hello));
 * ```
 */
#define E_SV_FMT ".*s"

/**
 * Helper macro for formatting string views in printf-style functions.
 *
 * See E_SV_FMT.
 */
#define E_SV_ARG(sv) (int) (sv).len, (sv).ptr

/**
 * Helper macro for creating a string view from a null-terminated string.
 *
 * The length of the string is determined using `sizeof (cstr) - 1`.
 *
 * Note that this **only** works if cstr is a **string literal**! If you have a regular pointer,
 * use `e_sv_from_cstr()` instead.
 */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define E_SV(cstr) ((E_Sv) {.ptr = (cstr), .len = sizeof (cstr) - 1})
#endif

E_Sv e_sv_from_parts (const char *ptr, size_t len);
E_Sv e_sv_from_cstr (const char *cstr);
int e_sv_eq (E_Sv a, E_Sv b);
int e_sv_starts_with (E_Sv haystack, E_Sv needle);
int e_sv_ends_with (E_Sv haystack, E_Sv needle);
E_Sv e_sv_chop_left (E_Sv *sv, size_t n);
E_Sv e_sv_chop_right (E_Sv *sv, size_t n);

/**************************************************************************************************/

#ifdef E_SV_IMPL

# include <stddef.h>

/**
 * Construct a string view from a pointer and an associated length. `ptr` may not be `NULL`.
 */
E_Sv
e_sv_from_parts (const char *ptr, size_t len)
{
    E_Sv sv;
    sv.ptr = ptr;
    sv.len = len;
    return sv;
}

/**
 * Construct a string view from a null-terminated string.
 *
 * The length of the string is determined using `strlen()`. `cstr` may not be `NULL`.
 */
E_Sv
e_sv_from_cstr (const char *cstr)
{
    size_t len = 0;
    while (cstr[len] != '\0')
        len += 1;
    return e_sv_from_parts (cstr, len);
}

/**
 * Check if two string views are equal. If they are equal, non-zero is returned.
 */
int
e_sv_eq (E_Sv a, E_Sv b)
{
    size_t i;
    if (a.len != b.len) return 0;
    for (i = 0; i < a.len; i++) {
        if (a.ptr[i] != b.ptr[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Check if a string view starts with a specific text. If `haystack` starts with `needle`,
 * non-zero is returned.
 */
int
e_sv_starts_with (E_Sv haystack, E_Sv needle)
{
    if (haystack.len < needle.len) return 0;
    haystack.len = needle.len;
    return e_sv_eq (haystack, needle);
}

/**
 * Check if a string view ends with a specific text. If `haystack` ends with `needle`,
 * non-zero is returned.
 */
int
e_sv_ends_with (E_Sv haystack, E_Sv needle)
{
    size_t diff;
    if (haystack.len < needle.len) return 0;
    diff = haystack.len - needle.len;
    haystack.ptr += diff;
    haystack.len = needle.len;
    return e_sv_eq (haystack, needle);
}

/**
 * Remove n bytes from the start of the string view.
 *
 * This function modifies the passed string view and returns the part of the
 * string that was chopped off.
 */
E_Sv
e_sv_chop_left (E_Sv *sv, size_t n)
{
    E_Sv res;
    if (n > sv->len) n = sv->len;
    res = e_sv_from_parts (sv->ptr, n);
    sv->ptr += n;
    sv->len -= n;
    return res;
}

/**
 * Remove n bytes from the end of the string view.
 *
 * This function modifies the passed string view and returns the part of the
 * string that was chopped off.
 */
E_Sv
e_sv_chop_right (E_Sv *sv, size_t n)
{
    E_Sv res;
    if (n > sv->len) n = sv->len;
    sv->len -= n;
    res = e_sv_from_parts (sv->ptr + sv->len, n);
    return res;
}

#endif /* E_SV_IMPL */

#endif /* E_SV_H_ */
