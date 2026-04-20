#ifndef EMPOWER_COMPAT_H_
#define EMPOWER_COMPAT_H_

/**************************************************************************************************
 *
 * This module provides macros for compatibility across C versions and compilers.
 *
 * It essentially aims to provide a unified interface for compiler-specific extensions and
 * workarounds that are required for old C versions.
 *
 * Since this file is quite messed up due to macro insanity, here is a list of the offered macros:
 *   - `E_ALIGNOF`       (== `alignof`)
 *   - `E_ALIGN_MAX`     (== `sizeof (max_align_t)`)
 *   - `E_TYPEOF`        (== `typeof`)
 *   - `E_TYPEOF_UNQUAL` (== `typeof_unqual`)
 *
 **************************************************************************************************/

/* clang-format off */
#if !defined (E_ALIGNOF)
# if __STDC_VERSION__ >= 202311L
#  define E_ALIGNOF(expr) alignof (expr)
# elif __STDC_VERSION__ >= 201112L
#  define E_ALIGNOF(expr) _Alignof (expr)
# elif defined (__GNUC__) || defined (__clang__) || defined (__TINYC__)
#  define E_ALIGNOF(expr) __alignof__ (expr)
# elif defined (_MSC_VER)
#  define E_ALIGNOF(expr) __alignof (expr)
# else
#  define E_ALIGNOF(type) ((char *) (&((struct { char c; type t; } *) 0)->t) - (char *) 0)
# endif
#endif
/* clang-format on */

/* clang-format off */
#ifndef E_ALIGN_MAX
# if (__STDC_VERSION__ >= 201112L && !defined(_MSC_VER))
#  define E_ALIGN_MAX E_ALIGNOF (max_align_t)
# elif defined(__GNUC__) || defined(__clang__)
#  define E_ALIGN_MAX __BIGGEST_ALIGNMENT__
# else
union e_max_align_union {
    long dummy1;
    double dummy2;
    void *dummy3;
    void (*dummy4) (void);
#  if __STDC_VERSION__ >= 199901L
    long long dummy5;
    long double dummy6;
#  endif
};
#  define E_ALIGN_MAX E_ALIGNOF (union e_max_align_union)
# endif
#endif
/* clang-format on */

#ifndef E_TYPEOF
# if __STDC_VERSION__ >= 202311L
#  define E_TYPEOF(x) typeof (x)
# else
#  define E_TYPEOF(x) __typeof__ (x)
# endif
#endif /* E_TYPEOF */

#ifndef E_TYPEOF_UNQUAL
# if __STDC_VERSION__ >= 202311L
#  define E_TYPEOF_UNQUAL(x) typeof_unqual (x)
# else
#  define E_TYPEOF_UNQUAL(x) __typeof_unqual__ (x)
# endif
#endif /* E_TYPEOF_UNQUAL */

#endif /* EMPOWER_COMPAT_H_ */
