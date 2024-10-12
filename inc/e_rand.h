#ifndef _EMPOWER_RAND_H_
#define _EMPOWER_RAND_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_RAND

/*! e_rand ********************************************************************
 * 
 * This module provides functions related to randomization.
 *
 ******************************************************************************/

void e_math_rand_seed (u32 seed);
void e_math_rand_seed_time (void);
u32 e_math_rand_u32 (void);
u64 e_math_rand_u64 (void);
f32 e_math_rand_f32 (void);
f64 e_math_rand_f64 (void);

#endif /* E_CONFIG_MODULE_RAND */
#endif /* _EMPOWER_RAND_H_ */
