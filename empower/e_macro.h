#ifndef EMPOWER_MACRO_H_
#define EMPOWER_MACRO_H_

/******************************************************************************
 *
 * This module provides a number of utility marcos.
 *
 ******************************************************************************/

#include <stddef.h>

/**
 * Turn any expression into a string.
 */
#ifndef E_STRINGIFY
# define E_STRINGIFY_HELPER_(x) #x
# define E_STRINGIFY(x) E_STRINGIFY_HELPER_ (x)
#endif

/**
 * Source file and line concatenated into a string like "main.c:42"
 */
#ifndef E_SOURCE_LOCATION
# define E_SOURCE_LOCATION __FILE__ ":" E_STRINGIFY (__LINE__)
#endif

/**
 * Concatenate any two expressions.
 */
#ifndef E_MACRO_CONCAT
# define E_MACRO_CONCAT_HELPER_(a, b) a##b
# define E_MACRO_CONCAT(a, b) E_MACRO_CONCAT_HELPER_ (a, b)
#endif

/**
 * Obtain the parent of type \type from the pointer \ptr to its member called
 * \member.
 */
#define E_CONTAINER_OF(ptr, type, member) ((type *) ((char *) (ptr) - offsetof (type, member)))

/**
 * Obtain the number of items in an array. Does not work with pointers, only
 * arrays.
 */
#define E_ARRAY_SIZE(arr) (sizeof (arr) / sizeof ((arr)[0]))

/**
 * The `E_MACRO_DECL_STRINGIFY_ENUM` and `E_MACRO_IMPL_STRINGIFY_ENUM` can be
 * used to create a quick and dirty automatic enum stringification function. The
 * generated stringification function returns the literal name as a string. In
 * the case of an invalid enum value, NULL is returned.
 *
 * Example:

 *     // ----- (orientation.h) -----
 *     #define ORIENTATION_ENUM_DEF_(X)  \
 *             X(ORIENTATION_NORTH) \
 *             X(ORIENTATION_EAST)  \
 *             X(ORIENTATION_SOUTH) \
 *             X(ORIENTATION_WEST)
 *     E_MACRO_DECL_STRINGIFY_ENUM (ORIENTATION_ENUM_DEF_, orientation, orientation_to_str);
 *   
 *     // ----- (orientation.c) -----
 *     #include "orientation.h"
 *     E_MACRO_IMPL_STRINGIFY_ENUM (ORIENTATION_ENUM_DEF_, orientation, orientation_to_str)
 *
 * Generated output of example:
 *
 *     // ----- (orientation.h) -----
 *     enum orientation {
 *             ORIENTATION_NORTH,
 *             ORIENTATION_EAST,
 *             ORIENTATION_SOUTH,
 *             ORIENTATION_WEST,
 *     };
 *     const char *orientation_to_str (enum orientation value);
 *  
 *     // ----- (orientation.c) -----
 *     const char *
 *     orientation_to_str (enum orientation value)
 *     {
 *             switch (value) {
 *                     case ORIENTATION_NORTH: return "ORIENTATION_NORTH";
 *                     case ORIENTATION_EAST:  return "ORIENTATION_EAST";
 *                     case ORIENTATION_SOUTH: return "ORIENTATION_SOUTH";
 *                     case ORIENTATION_WEST:  return "ORIENTATION_WEST";
 *                     default:                return NULL;
 *             }
 *     }
 */
#if __STDC_VERSION__ >= 199901L
# define E_MACRO_ENUM_VALUE_(name) name,
# define E_MACRO_ENUM_CASE_(name) case name: return #name;
# define E_MACRO_DECL_STRINGIFY_ENUM(ENUM_DEF, enum_type_name, stringify_function) \
	enum enum_type_name { \
		ENUM_DEF (E_MACRO_ENUM_VALUE_) \
	}; \
	const char *stringify_function (enum enum_type_name value)
# define E_MACRO_IMPL_STRINGIFY_ENUM(ENUM_DEF, enum_type_name, stringify_function) \
	const char * \
	stringify_function (enum_type_name value) \
	{ \
		switch (value) { \
			ENUM_DEF (E_MACRO_ENUM_CASE_) \
			default: return NULL; \
		} \
	}
#endif

/**
 * Obtain the formatting argument for printf-like functions for the generic
 * argument `value`. Works for regular integers and floats. All other types are
 * treated as pointers.
 */
#if __STDC_VERSION__ >= 201112L
# ifndef E_AUTO_FMT
#  define E_AUTO_FMT(value) _Generic ((value), \
        char: "%c", \
        signed char: "%d", \
        signed short: "%d", \
        signed int: "%d", \
        signed long: "%ld", \
        signed long long: "%lld", \
        unsigned char: "%u", \
        unsigned short: "%u", \
        unsigned int: "%u", \
        unsigned long: "%lu", \
        unsigned long long: "%llu", \
        float: "%f", \
        double: "%g", \
        long double: "%lg", \
        _Bool: "%d", \
        default: "%p")
# endif
#endif

#endif /* EMPOWER_MACRO_H_ */
