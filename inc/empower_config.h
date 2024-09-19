#ifndef _EMPOWER_CONFIG_H_
#define _EMPOWER_CONFIG_H_

/* modules ********************************************************************/
#define E_CONFIG_MODULE_ALLOC				1
#define E_CONFIG_MODULE_CSTR				1
#define E_CONFIG_MODULE_DEBUG				1
#define E_CONFIG_MODULE_LOG				1
#define E_CONFIG_MODULE_RESULT				1
#define E_CONFIG_MODULE_TEST				1
#define E_CONFIG_MODULE_VEC				1

/* library settings ***********************************************************/
#define E_CONFIG_TEST_TYPE_MACROS			0

/* compiler and language specification ****************************************/
#define E_CONFIG_HAS_SYS_TYPES				1
#define E_CONFIG_HAS_INT_128				0
#define E_CONFIG_HAS_FLOAT				1
#define E_CONFIG_HAS_DOUBLE				1
#define E_CONFIG_HAS_LONG_DOUBLE			1
#define E_CONFIG_C89_HAS_RESTRICT			1
#define E_CONFIG_C89_INT_IS_32_BIT			1
#define E_CONFIG_C89_LONG_IS_64_BIT			1

#endif /* _EMPOWER_CONFIG_H_ */
