#ifndef _EMPOWER_COMPILER_H_
#define _EMPOWER_COMPILER_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */

/*! e_compiler ****************************************************************
 * 
 * This module provides macros for interacting with different compilers.
 *
 * Docs for available compiler attributes can be found here:
 *
 *  - GCC:
 *     - https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
 *     - https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html
 *     - https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html
 *     - https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html
 *     - https://gcc.gnu.org/onlinedocs/gcc/Enumerator-Attributes.html
 *     - https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html
 *  - Clang:
 *     - https://clang.llvm.org/docs/AttributeReference.html
 *  - MSVC:
 *     - https://learn.microsoft.com/en-us/cpp/cpp/declspec?view=msvc-170
 *
 ******************************************************************************/

#if defined (__clang__)
# define E_COMPILER_CLANG
#elif defined (_MSC_VER) /* defined (__clang__) */
# define E_COMPILER_MSVC
#elif defined (__GNUC__) /* defined (_MSC_VER) */
# define E_COMPILER_GCC
#endif /* defined (__GNUC__) */

#if E_STDC_VERSION >= E_STDC_VERSION_C23
# define __E_ATTRIBUTE_C(attr) [[attr]]
# define __E_ATTRIBUTE_C_NO_MSVC(attr) [[attr]]
#elif defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC) /* E_STDC_VERSION >= E_STDC_VERSION_C23 */
# define __E_ATTRIBUTE_C(attr) __attribute__ ((attr))
# define __E_ATTRIBUTE_C_NO_MSVC(attr) __attribute__ ((attr))
#elif defined (E_COMPILER_MSVC) /* defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC) */
# define __E_ATTRIBUTE_C(attr) __declspec (attr)
# define __E_ATTRIBUTE_C_NO_MSVC(attr)
#else /* defined (E_COMPILER_MSVC) */
# define __E_ATTRIBUTE_C(attr)
# define __E_ATTRIBUTE_C_NO_MSVC(attr)
#endif /* defined (E_COMPILER_MSVC) */

/* === c attributes ========================================================= */

#define E_ATTR_DEPRECATED __E_ATTRIBUTE_C (deprecated)
#define E_ATTR_DEPRECATED_MSG(msg) __E_ATTRIBUTE_C (deprecated (msg))
#define E_ATTR_FALLTHROUGH __E_ATTRIBUTE_C_NO_MSVC (fallthrough)
#define E_ATTR_REPRODUCIBLE __E_ATTRIBUTE_C_NO_MSVC (reproducible)
#define E_ATTR_UNSEQUENCED __E_ATTRIBUTE_C_NO_MSVC (unsequenced)

#if (E_STDC_VERSION >= E_STDC_VERSION_C11) && (E_STDC_VERSION < E_STDC_VERSION_C23) 
# define E_ATTR_NORETURN _Noreturn
#else /* (E_STDC_VERSION >= E_STDC_VERSION_C11) && (E_STDC_VERSION < E_STDC_VERSION_C23) */
# define E_ATTR_NORETURN __E_ATTRIBUTE_C (noreturn)
#endif /* (E_STDC_VERSION >= E_STDC_VERSION_C11) && (E_STDC_VERSION < E_STDC_VERSION_C23) */

#if E_STDC_VERSION >= E_STDC_VERSION_C23
# define E_ATTR_UNUSED [[maybe_unused]]
#else /* E_STDC_VERSION >= E_STDC_VERSION_C23 */
# define E_ATTR_UNUSED __E_ATTRIBUTE_C_NO_MSVC (used)
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C23 */

#if E_STDC_VERSION >= E_STDC_VERSION_C23
# define E_ATTR_NODISCARD [[nodiscard]]
#else /* E_STDC_VERSION >= E_STDC_VERSION_C23 */
# define E_ATTR_NODISCARD __E_ATTRIBUTE_C_NO_MSVC (warn_unused_result)
#endif /* E_STDC_VERSION >= E_STDC_VERSION_C23 */

/* === common attributes ==================================================== */

#if defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC)
# define __E_ATTRIBUTE_COMMON(attr, msvc) __attribute__ ((attr))
#elif defined (E_COMPILER_MSVC) /* defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC) */
# define __E_ATTRIBUTE_COMMON(attr, msvc) __declspec (msvc)
#else /* defined (E_COMPILER_MSVC) */
# define __E_ATTRIBUTE_COMMON(msvc)
#endif /* defined (E_COMPILER_MSVC) */

#define E_ATTR_NOINLINE __E_ATTRIBUTE_COMMON (noinline, noinline)
#define E_ATTR_MALLOC __E_ATTRIBUTE_COMMON (malloc, allocator)
#define E_ATTR_MALLOC_WITH_DEALLOC(deallocator) __E_ATTRIBUTE_COMMON (malloc (deallocator), allocator)
#define E_ATTR_MALLOC_WITH_DEALLOC_IDX(deallocator, ptr_idx) __E_ATTRIBUTE_COMMON (malloc (deallocator, ptr_idx), allocator)
#define E_ATTR_UNAVAILABLE __E_ATTRIBUTE_COMMON (unavailable, deprecated)
#define E_ATTR_UNAVAILABLE_MSG(msg) __E_ATTRIBUTE_COMMON (unavailable (msg), deprecated (msg))

#if defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC)
# define E_ATTR_ALWAYS_INLINE __attribute__ ((always_inline))
#elif defined (E_COMPILER_MSVC) /* defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC) */
# define E_ATTR_ALWAYS_INLINE __forceinline
#else /* defined (E_COMPILER_MSVC) */
# define E_ATTR_ALWAYS_INLINE
#endif /* defined (E_COMPILER_MSVC) */

/* === gcc + clang only attributes ========================================== */

#if defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC)
# define __E_ATTRIBUTE_GCC_CLANG_ONLY(attr) __attribute__ ((attr))
#else /* defined (E_COMPILER_CLANG) || defined (E_COMPILER_GCC) */
# define __E_ATTRIBUTE_GCC_CLANG_ONLY(attr)
#endif /* defined (E_COMPILER_MSVC) */

#define E_ATTR_ALLOC_ALIGN(param_num) __E_ATTRIBUTE_GCC_CLANG_ONLY (alloc_align (param_num))
#define E_ATTR_ALLOC_SIZE(param_num) __E_ATTRIBUTE_GCC_CLANG_ONLY (alloc_size (param_num))
#define E_ATTR_ALLOC_SIZE_MULT(param_num_1, param_num_2) __E_ATTRIBUTE_GCC_CLANG_ONLY (alloc_size (param_num_1, param_num_2))
#define E_ATTR_ASSUME(expr) __E_ATTRIBUTE_GCC_CLANG_ONLY (assume (expr))
#define E_ATTR_ASSUME_ALIGNED(alignment) __E_ATTRIBUTE_GCC_CLANG_ONLY (assume_aligned (alignment))
#define E_ATTR_ASSUME_ALIGNED_WITH_OFFSET(alignment, offset) __E_ATTRIBUTE_GCC_CLANG_ONLY (assume_aligned (alignment, offset))
#define E_ATTR_COLD __E_ATTRIBUTE_GCC_CLANG_ONLY (cold)
#define E_ATTR_CONST __E_ATTRIBUTE_GCC_CLANG_ONLY (const)
#define E_ATTR_COUNTED_BY(field) __E_ATTRIBUTE_GCC_CLANG_ONLY (counted_by (field))
#define E_ATTR_ERROR(msg) __E_ATTRIBUTE_GCC_CLANG_ONLY (error (msg))
#define E_ATTR_FLAG_ENUM __E_ATTRIBUTE_GCC_CLANG_ONLY (flag_enum)
#define E_ATTR_FLATTEN __E_ATTRIBUTE_GCC_CLANG_ONLY (flatten)
#define E_ATTR_FORMAT(func, fmt, va_param) __E_ATTRIBUTE_GCC_CLANG_ONLY (format (func, fmt, va_param))
#define E_ATTR_HOT __E_ATTRIBUTE_GCC_CLANG_ONLY (hot)
#define E_ATTR_LIKELY __E_ATTRIBUTE_GCC_CLANG_ONLY (likely)
#define E_ATTR_NONNULL(param_num) __E_ATTRIBUTE_GCC_CLANG_ONLY (nonnull (param_num))
#define E_ATTR_NONNULL_2(param_num_1, param_num_2) __E_ATTRIBUTE_GCC_CLANG_ONLY (nonnull (param_num_1, param_num_2))
#define E_ATTR_NONNULL_3(param_num_1, param_num_2, param_num_3) __E_ATTRIBUTE_GCC_CLANG_ONLY (nonnull (param_num_1, param_num_2, param_num_3))
#define E_ATTR_NONNULL_4(param_num_1, param_num_2, param_num_3, param_num_4) __E_ATTRIBUTE_GCC_CLANG_ONLY (nonnull (param_num_1, param_num_2, param_num_3, param_num_4))
#define E_ATTR_PACKED __E_ATTRIBUTE_GCC_CLANG_ONLY (packed)
#define E_ATTR_PURE __E_ATTRIBUTE_GCC_CLANG_ONLY (pure)
#define E_ATTR_RETURNS_NONNULL __E_ATTRIBUTE_GCC_CLANG_ONLY (returns_nonnull)
#define E_ATTR_UNLIKELY __E_ATTRIBUTE_GCC_CLANG_ONLY (unlikely)
#define E_ATTR_WARNING(msg) __E_ATTRIBUTE_GCC_CLANG_ONLY (warning (msg))
#define E_ATTR_WEAK __E_ATTRIBUTE_GCC_CLANG_ONLY (weak)

/* === gcc only attributes ================================================== */

#if defined (E_COMPILER_GCC)
# define __E_ATTRIBUTE_GCC_ONLY(attr) __attribute__ ((attr))
#else /* defined (E_COMPILER_GCC) */
# define __E_ATTRIBUTE_GCC_ONLY(attr)
#endif /* defined (E_COMPILER_MSVC) */

#define E_ATTR_ACCESS_READ_ONLY(param_num) __E_ATTRIBUTE_GCC_ONLY (access (read_only, param_num))
#define E_ATTR_ACCESS_READ_ONLY_LEN(param_num, len_param_num) __E_ATTRIBUTE_GCC_ONLY (access (read_only, param_num, len_param_num))
#define E_ATTR_ACCESS_WRITE_ONLY(param_num) __E_ATTRIBUTE_GCC_ONLY (access (write_only, param_num))
#define E_ATTR_ACCESS_WRITE_ONLY_LEN(param_num, len_param_num) __E_ATTRIBUTE_GCC_ONLY (access (write_only, param_num, len_param_num))
#define E_ATTR_ACCESS_READ_WRITE(param_num) __E_ATTRIBUTE_GCC_ONLY (access (read_write, param_num))
#define E_ATTR_ACCESS_READ_WRITE_LEN(param_num, len_param_num) __E_ATTRIBUTE_GCC_ONLY (access (read_write, param_num, len_param_num))
#define E_ATTR_NUL_STRING_ARG(param_num) __E_ATTRIBUTE_GCC_ONLY (null_terminated_string_arg (param_num))
#define E_ATTR_NUL_STRING_ARG_2(param_num_1, param_num_2) __E_ATTRIBUTE_GCC_ONLY (null_terminated_string_arg (param_num_1, param_num_2))
#define E_ATTR_NUL_STRING_ARG_3(param_num_1, param_num_2, param_num_3) __E_ATTRIBUTE_GCC_ONLY (null_terminated_string_arg (param_num_1, param_num_2, param_num_3))
#define E_ATTR_NUL_STRING_ARG_4(param_num_1, param_num_2, param_num_3, param_num_4) __E_ATTRIBUTE_GCC_ONLY (null_terminated_string_arg (param_num_1, param_num_2, param_num_3, param_num_4))

/* === clang only attributes ================================================ */

#if defined (E_COMPILER_CLANG)
# define __E_ATTRIBUTE_CLANG_ONLY(attr) __attribute__ ((attr))
#else /* defined (E_COMPILER_CLANG) */
# define __E_ATTRIBUTE_CLANG_ONLY(attr)
#endif /* defined (E_COMPILER_MSVC) */

#define E_ATTR_ACQUIRE_HANDLE(name) __E_ATTRIBUTE_CLANG_ONLY (acquire_handle (name))
#define E_ATTR_ALIGN_VALUE(alignment) __E_ATTRIBUTE_CLANG_ONLY (align_value (alignment))
#define E_ATTR_ALLOCATING __E_ATTRIBUTE_CLANG_ONLY (allocating)
#define E_ATTR_BLOCKING __E_ATTRIBUTE_CLANG_ONLY (blocking)
#define E_ATTR_CALLBACK(func_ptr_param, param) __E_ATTRIBUTE_CLANG_ONLY (callback (func_ptr_param, param))
#define E_ATTR_CALLBACK_2(func_ptr_param, param_1, param_2) __E_ATTRIBUTE_CLANG_ONLY (callback (func_ptr_param, param_1, param_2))
#define E_ATTR_CALLBACK_3(func_ptr_param, param_1, param_2, param_3) __E_ATTRIBUTE_CLANG_ONLY (callback (func_ptr_param, param_1, param_2, param_3))
#define E_ATTR_CALLBACK_4(func_ptr_param, param_1, param_2, param_3, param_4) __E_ATTRIBUTE_CLANG_ONLY (callback (func_ptr_param, param_1, param_2, param_3, param_4))
#define E_ATTR_CALLED_ONCE __E_ATTRIBUTE_CLANG_ONLY (called_once)
#define E_ATTR_DIAGNOSE_AS_BUILTIN(builtin_func) __E_ATTRIBUTE_CLANG_ONLY (diagnose_as_builtin (builtin_func))
#define E_ATTR_DIAGNOSE_AS_BUILTIN_1(builtin_func, param) __E_ATTRIBUTE_CLANG_ONLY (diagnose_as_builtin (builtin_func, param))
#define E_ATTR_DIAGNOSE_AS_BUILTIN_2(builtin_func, param_1, param_2) __E_ATTRIBUTE_CLANG_ONLY (diagnose_as_builtin (builtin_func, param_1, param_2))
#define E_ATTR_DIAGNOSE_AS_BUILTIN_3(builtin_func, param_1, param_2, param_3) __E_ATTRIBUTE_CLANG_ONLY (diagnose_as_builtin (builtin_func, param_1, param_2, param_3))
#define E_ATTR_DIAGNOSE_AS_BUILTIN_4(builtin_func, param_1, param_2, param_3, param_4) __E_ATTRIBUTE_CLANG_ONLY (diagnose_as_builtin (builtin_func, param_1, param_2, param_3, param_4))
#define E_ATTR_DIAGNOSE_IF(condition, msg, error_or_warning) __E_ATTRIBUTE_CLANG_ONLY (diagnose_if (condition, msg, error_or_warning))
#define E_ATTR_ENUM_CLOSED __E_ATTRIBUTE_CLANG_ONLY (enum_extensibility (closed))
#define E_ATTR_ENUM_OPEN __E_ATTRIBUTE_CLANG_ONLY (enum_extensibility (open))
#define E_ATTR_NO_BUILTIN(builtin_func) __E_ATTRIBUTE_CLANG_ONLY (no_builtin (builtin_func))
#define E_ATTR_NO_DEREF __E_ATTRIBUTE_CLANG_ONLY (no_builtin (noderef))
#define E_ATTR_NON_ALLOCATING __E_ATTRIBUTE_CLANG_ONLY (no_builtin (nonallocating))
#define E_ATTR_NON_BLOCKING __E_ATTRIBUTE_CLANG_ONLY (no_builtin (nonblocking))
#define E_ATTR_PREFERRED_TYPE(type) __E_ATTRIBUTE_CLANG_ONLY (no_builtin (preferred_type (type)))
#define E_ATTR_RELEASE_HANDLE(name) __E_ATTRIBUTE_CLANG_ONLY (no_builtin (release_handle (name)))
#define E_ATTR_USE_HANDLE(name) __E_ATTRIBUTE_CLANG_ONLY (no_builtin (use_handle (name)))

/* TODO: align/alignas stuff */

#endif /* _EMPOWER_COMPILER_H_ */
