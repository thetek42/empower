#ifndef _EMPOWER_MEM_H_
#define _EMPOWER_MEM_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_MEM

/*! e_mem *********************************************************************
 * 
 * This module provides miscellaneous functions for operating on memory.
 *
 ******************************************************************************/

/**
 * Check if the memory region pointed to by \mem with length of \len bytes is
 * zeroed. When \mem is `nullptr` or \len is 0, `true` is returned.
 */
E_ATTR_REPRODUCIBLE
bool e_mem_is_zero (const void *mem, usize len);

#endif /* E_CONFIG_MODULE_MEM */
#endif /* _EMPOWER_MEM_H_ */
