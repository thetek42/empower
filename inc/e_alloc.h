#ifndef _EMPOWER_ALLOC_H_
#define _EMPOWER_ALLOC_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_ALLOC

#define e_alloc(type, nmemb) __e_mem_alloc (sizeof (type) * (nmemb))
#define e_alloc_size(size) __e_mem_alloc ((size))
#define e_alloc_zero(type, nmemb) __e_mem_calloc ((nmemb), sizeof (type))
#define e_alloc_zero_size(size, nmemb) __e_mem_calloc ((nmemb), (size))
#define e_realloc(ptr, type, nmemb) __e_mem_realloc ((ptr), sizeof (type) * (nmemb))
#define e_realloc_size(ptr, size) __e_mem_realloc ((ptr), (size))
#define e_free(ptr) free ((ptr))
#define e_new(type) e_alloc (type, 1)
#define e_new_zero(type) e_alloc_zero (type, 1)

void *__e_mem_alloc (usize size);
void *__e_mem_calloc (usize nmemb, usize size);
void *__e_mem_realloc (void *ptr, usize size);

#endif /* E_CONFIG_MODULE_ALLOC */
#endif /* _EMPOWER_ALLOC_H_ */
