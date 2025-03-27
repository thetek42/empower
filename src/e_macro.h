#ifndef _EMPOWER_MACRO_H_
#define _EMPOWER_MACRO_H_

/*! e_macro *******************************************************************
 *
 * This module provides macro utilities.
 *
 ******************************************************************************/

/**
 * Turn any expression into a string.
 */
#define __E_MACRO_STRINGIFY_HELPER(x) #x
#define E_MACRO_STRINGIFY(x) __E_MACRO_STRINGIFY_HELPER (x)

/**
 * Concatenate any two expressions.
 */
#define __E_MACRO_CONCAT_HELPER(a, b) a##b
#define E_MACRO_CONCAT(a, b) __E_MACRO_CONCAT_HELPER (a, b)

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
 * Expands __FILE__ and __LINE__ to a string like "main.c:42"
 */
#define E_MACRO_FILE_LINE __FILE__ ":" E_MACRO_STRINGIFY (__LINE__)

/**
 * The `E_MACRO_DECL_STRINGIFY_ENUM` and `E_MACRO_IMPL_STRINGIFY_ENUM` can be
 * used to create a quick and dirty automatic enum stringification function. The
 * generated stringification function returns the literal name as a string. In
 * the case of an invalid enum value, `nullptr` is returned.
 *
 * Example:
 *  | // ----- (orientation.h) -----
 *  | #define ORIENTATION_ENUM(X)  \
 *  |         X(Orientation_North) \
 *  |         X(Orientation_East)  \
 *  |         X(Orientation_South) \
 *  |         X(Orientation_West)
 *  | E_MACRO_DECL_STRINGIFY_ENUM (ORIENTATION_ENUM, Orientation, orientation_to_str);
 *  |
 *  | // ----- (orientation.c) -----
 *  | #include "orientation.h"
 *  | E_MACRO_IMPL_STRINGIFY_ENUM (ORIENTATION_ENUM, Orientation, orientation_to_str)
 *
 * Generated output of example:
 *  | // ----- (orientation.h) -----
 *  | typedef enum {
 *  |         Orientation_North,
 *  |         Orientation_East,
 *  |         Orientation_South,
 *  |         Orientation_West,
 *  | } Orientation;
 *  | const char *orientation_to_str (Orientation value);
 *  |
 *  | // ----- (orientation.c) -----
 *  | const char *
 *  | orientation_to_str (Orientation value)
 *  | {
 *  |         switch (value) {
 *  |                 case Orientation_North: { return "Orientation_North"; }
 *  |                 case Orientation_East:  { return "Orientation_East";  }
 *  |                 case Orientation_South: { return "Orientation_South"; }
 *  |                 case Orientation_West:  { return "Orientation_West";  }
 *  |                 default:                { return NULL;                }
 *  |         }
 *  | }
 */
#define E_MACRO_DECL_STRINGIFY_ENUM(ENUM_DEF, enum_type_name, stringify_function) \
	typedef enum {                                                         \
		ENUM_DEF (__E_MACRO_ENUM_VALUE)                                \
	} enum_type_name;                                                      \
	const char *stringify_function (enum_type_name value)

/*
 * See `E_MACRO_DECL_STRINGIFY_ENUM`
 */
#define E_MACRO_IMPL_STRINGIFY_ENUM(ENUM_DEF, enum_type_name, stringify_function) \
	const char *                                                           \
	stringify_function (enum_type_name value)                              \
	{                                                                      \
		switch (value) {                                               \
			ENUM_DEF (__E_MACRO_ENUM_CASE)                         \
			default: { return NULL; }                              \
		}                                                              \
	}

#define __E_MACRO_ENUM_VALUE(name) name,
#define __E_MACRO_ENUM_CASE(name) case name: { return #name; }

#endif /* _EMPOWER_MACRO_H_ */
