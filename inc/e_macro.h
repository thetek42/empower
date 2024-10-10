#ifndef _EMPOWER_MACRO_H_
#define _EMPOWER_MACRO_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_macro *******************************************************************
 * 
 * This module provides macro utilities.
 *
 ******************************************************************************/

/**
 * Turn any expression into a string.
 */
#define E_MACRO_STRINGIFY(x) __E_MACRO_STRINGIFY_HELPER (x)
#define __E_MACRO_STRINGIFY_HELPER(x) #x

/**
 * Concatenate any two expressions.
 */
#define E_MACRO_CONCAT(a, b) __E_MACRO_CONCAT_HELPER (a, b)
#define __E_MACRO_CONCAT_HELPER(a, b) a##b

/**
 * Obtain the parent of type \type from the pointer \ptr to its member called
 * \member.
 */
#define E_CONTAINER_OF(ptr, type, member) ((type *) ((char *) (ptr) - offsetof (type, member)))

/**
 * Expands __FILE__ and __LINE__ to a string like "main.c:42"
 */
#define E_MACRO_FILE_LINE __FILE__ ":" E_MACRO_STRINGIFY (__LINE__)

#endif /* _EMPOWER_MACRO_H_ */
