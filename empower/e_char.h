#ifndef E_CHAR_H_
#define E_CHAR_H_

/**************************************************************************************************
 *
 * Empower / e_char.h - Public Domain - https://git.tjdev.de/thetek/empower
 *
 * A `<ctype.h>` that doesn’t suck. Refer to https://nullprogram.com/blog/2023/02/11/ for more
 * information. These functions do not depend on locale (they always use the default "C" locale),
 * and they do not have any weird behaviour when `char` is signed.
 *
 * All functions return `true` (non-zero) when they match, or `false` (zero) otherwise.
 *
 **************************************************************************************************/

int e_char_isalnum (char c);
int e_char_isalpha (char c);
int e_char_isascii (char c);
int e_char_isblank (char c);
int e_char_iscntrl (char c);
int e_char_isdigit (char c);
int e_char_isgraph (char c);
int e_char_islower (char c);
int e_char_isprint (char c);
int e_char_ispunct (char c);
int e_char_isspace (char c);
int e_char_isupper (char c);
int e_char_isxdigit (char c);

/**************************************************************************************************/

#ifdef E_CHAR_IMPL

int
e_char_isalnum (char c)
{
    return e_char_isalpha (c) || e_char_isdigit (c);
}

int
e_char_isalpha (char c)
{
    return e_char_islower (c) || e_char_isupper (c);
}

int
e_char_isascii (char c)
{
    return 0 <= c && c <= 127;
}

int
e_char_isblank (char c)
{
    return c == ' ' || c == '\t';
}

int
e_char_iscntrl (char c)
{
    return (0 <= c && c < 0x20) || c == 0x7F;
}

int
e_char_isdigit (char c)
{
    return '0' <= c && c <= '9';
}

int
e_char_isgraph (char c)
{
    return 0x20 < c && c < 0x7F;
}

int
e_char_islower (char c)
{
    return 'a' <= c && c <= 'z';
}

int
e_char_isprint (char c)
{
    return e_char_isgraph (c) || c == ' ';
}

int
e_char_ispunct (char c)
{
    return !e_char_isspace (c) && !e_char_isalnum (c);
}

int
e_char_isspace (char c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

int
e_char_isupper (char c)
{
    return 'A' <= c && c <= 'Z';
}

int
e_char_isxdigit (char c)
{
    return e_char_isdigit (c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

#endif /* E_CHAR_IMPL */

#endif /* E_CHAR_H_ */
