#ifndef _EMPOWER_CSTR_H_
#define _EMPOWER_CSTR_H_
#ifndef _EMPOWER_H_
#error "do not include this file directly, only include empower.h"
#endif /* _EMPOWER_H_ */
#if E_CONFIG_MODULE_CSTR

typedef int (* e_char_predicate_t) (int c);

usize e_cstr_count_char (const char *s, char c);
usize e_cstr_count_func (const char *s, e_char_predicate_t func);
bool e_cstr_is_ascii (const char *s);
bool e_cstr_is_blank (const char *s);
bool e_cstr_matches_predicate (const char *s, e_char_predicate_t func);
char *e_cstr_to_ascii_lower (char *s);
char *e_cstr_to_ascii_upper (char *s);
char *e_cstr_to_ascii_lower_buf (const char *restrict src, char *restrict dest);
char *e_cstr_to_ascii_upper_buf (const char *restrict src, char *restrict dest);

int e_char_is_ascii (int c);

#endif /* E_CONFIG_MODULE_CSTR */
#endif /* _EMPOWER_CSTR_H_ */
