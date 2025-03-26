#define E_PARSE_IMPL
#include <e_parse.h>

#include <e_test.h>
#include <e_types.h>

void
test_parse (void)
{
	const char *orig = "int foo (void) {\n    return 42; \n}";
	const char *num1 = "42?";
	const char *num2 = " \t 42?";
	const char *num3 = "+42?";
	const char *num4 = "-42?";
	const char *num5 = "0x42?";
	const char *num6 = "042?";
	const char *num7 = "127?";
	const char *num8 = "128?";
	const char *num9 = "-128?";
	const char *num10 = "-129?";
	const char *num11 = "999999999999999999999999999999?";
	const char *num12 = "-999999999999999999999999999999?";
	const char *s;
	u8 uout;
	i8 iout;

	s = orig;
	/* s = "int foo (void) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_char 1 ret", e_parse_char (&s, 'i'), orig);
	e_test_assert_ptr_eq ("e_parse_char 1 ptr", s, orig + 1);
	/* s = "nt foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char 2 ret", e_parse_char (&s, 'i'));
	e_test_assert_ptr_eq ("e_parse_char 2 ptr", s, orig + 1);
	e_test_assert_ptr_eq ("e_parse_char_not 1 ret", e_parse_char_not (&s, 't'), orig + 1);
	e_test_assert_ptr_eq ("e_parse_char_not 1 ptr", s, orig + 2);
	/* s = "t foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_not 2 ret", e_parse_char_not (&s, 't'));
	e_test_assert_ptr_eq ("e_parse_char_not 2 ptr", s, orig + 2);
	e_test_assert_ptr_eq ("e_parse_char_pat 1 ret", e_parse_char_pat (&s, "ajtbe"), orig + 2);
	e_test_assert_ptr_eq ("e_parse_char_pat 1 ptr", s, orig + 3);
	/* s = " foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_pat 2 ret", e_parse_char_pat (&s, "ajtbe"));
	e_test_assert_ptr_eq ("e_parse_char_pat 2 ptr", s, orig + 3);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ret", e_parse_char_not_pat (&s, "abcdefg"), orig + 3);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ptr", s, orig + 4);
	/* s = "foo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_not_pat 2 ret", e_parse_char_not_pat (&s, "abcdefg"));
	e_test_assert_ptr_eq ("e_parse_char_not_pat 2 ptr", s, orig + 4);
	e_test_assert_ptr_eq ("e_parse_char_func 1 ret", e_parse_char_func (&s, isalpha), orig + 4);
	e_test_assert_ptr_eq ("e_parse_char_func 1 ptr", s, orig + 5);
	/* s = "oo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_func 2 ret", e_parse_char_func (&s, isdigit));
	e_test_assert_ptr_eq ("e_parse_char_func 2 ptr", s, orig + 5);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ret", e_parse_char_not_func (&s, isdigit), orig + 5);
	e_test_assert_ptr_eq ("e_parse_char_not_pat 1 ptr", s, orig + 6);
	/* s = "o (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_char_not_pat 2 ret", e_parse_char_not_func (&s, isalpha));
	e_test_assert_ptr_eq ("e_parse_char_not_pat 2 ptr", s, orig + 6);

	s = orig;
	/* s = "int foo (void) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_str_match 1 ret", e_parse_str_match (&s, "int f"), orig);
	e_test_assert_ptr_eq ("e_parse_str_match 1 ptr", s, orig + 5);
	/* s = "oo (void) {\n    return 42; \n} */
	e_test_assert_null ("e_parse_str_match 2 ret", e_parse_str_match (&s, "int f"));
	e_test_assert_ptr_eq ("e_parse_str_match 2 ptr", s, orig + 5);
	e_test_assert_ptr_eq ("e_parse_consume_while_char ret", e_parse_consume_while_char (&s, 'o'), orig + 5);
	e_test_assert_ptr_eq ("e_parse_consume_while_char ptr", s, orig + 7);
	/* s = " (void) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_while_pat ret", e_parse_consume_while_pat (&s, "(v )"), orig + 7);
	e_test_assert_ptr_eq ("e_parse_consume_while_pat ptr", s, orig + 10);
	/* s = "oid) {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_while_func ret", e_parse_consume_while_func (&s, isalpha), orig + 10);
	e_test_assert_ptr_eq ("e_parse_consume_while_func ptr", s, orig + 13);
	/* s = ") {\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_until_char ret", e_parse_consume_until_char (&s, '{'), orig + 13);
	e_test_assert_ptr_eq ("e_parse_consume_until_char ptr", s, orig + 15);
	/* s = "{\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_until_pat ret", e_parse_consume_until_pat (&s, "ert"), orig + 15);
	e_test_assert_ptr_eq ("e_parse_consume_until_pat ptr", s, orig + 21);
	/* s = "return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_until_func ret", e_parse_consume_until_func (&s, isspace), orig + 21);
	e_test_assert_ptr_eq ("e_parse_consume_until_func ptr", s, orig + 27);
	/* s = " 42; \n} */

	s = orig + 16;
	/* s = "\n    return 42; \n} */
	e_test_assert_ptr_eq ("e_parse_consume_whitespace ret", e_parse_consume_whitespace (&s), orig + 16);
	e_test_assert_ptr_eq ("e_parse_consume_whitespace ptr", s, orig + 21);
	/* s = "return 42; \n} */
	s += 10;
	/* s = " \n} */
	e_test_assert_ptr_eq ("e_parse_consume_whitespace_until_newline ret", e_parse_consume_whitespace_until_newline (&s), orig + 31);
	e_test_assert_ptr_eq ("e_parse_consume_whitespace_until_newline ptr", s, orig + 32);
	e_test_assert_ptr_eq ("e_parse_char \\n ret", e_parse_char (&s, '\n'), orig + 32);
	e_test_assert_ptr_eq ("e_parse_char \\n ptr", s, orig + 33);

	s = num1;
	uout = 0;
	e_test_assert_ptr_eq ("e_parse_u8 42 ret", e_parse_u8 (&s, &uout, 0), num1);
	e_test_assert_ptr_eq ("e_parse_u8 42 ptr", s, &num1[2]);
	e_test_assert_eq ("e_parse_u8 42 out", u8, uout, 42);

	s = num2;
	uout = 0;
	e_test_assert_ptr_eq ("e_parse_u8 space ret", e_parse_u8 (&s, &uout, 0), num2);
	e_test_assert_ptr_eq ("e_parse_u8 space ptr", s, &num2[5]);
	e_test_assert_eq ("e_parse_u8 space out", u8, uout, 42);

	s = num3;
	uout = 0;
	e_test_assert_ptr_eq ("e_parse_u8 +42 ret", e_parse_u8 (&s, &uout, 0), num3);
	e_test_assert_ptr_eq ("e_parse_u8 +42 ptr", s, &num3[3]);
	e_test_assert_eq ("e_parse_u8 +42 out", u8, uout, 42);

	s = num3;
	iout = 0;
	e_test_assert_ptr_eq ("e_parse_i8 +42 ret", e_parse_i8 (&s, &iout, 0), num3);
	e_test_assert_ptr_eq ("e_parse_i8 +42 ptr", s, &num3[3]);
	e_test_assert_eq ("e_parse_i8 +42 out", i8, iout, 42);

	s = num4;
	iout = 0;
	e_test_assert_ptr_eq ("e_parse_i8 -42 ret", e_parse_i8 (&s, &iout, 0), num4);
	e_test_assert_ptr_eq ("e_parse_i8 -42 ptr", s, &num4[3]);
	e_test_assert_eq ("e_parse_i8 -42 out", i8, iout, -42);

	s = num5;
	uout = 0;
	e_test_assert_ptr_eq ("e_parse_u8 0x42 ret", e_parse_u8 (&s, &uout, 0), num5);
	e_test_assert_ptr_eq ("e_parse_u8 0x42 ptr", s, &num5[4]);
	e_test_assert_eq ("e_parse_u8 0x42 out", u8, uout, 0x42);

	s = num6;
	uout = 0;
	e_test_assert_ptr_eq ("e_parse_u8 042 ret", e_parse_u8 (&s, &uout, 0), num6);
	e_test_assert_ptr_eq ("e_parse_u8 042 ptr", s, &num6[3]);
	e_test_assert_eq ("e_parse_u8 042 out", u8, uout, 042);

	s = num7;
	iout = 0;
	e_test_assert_ptr_eq ("e_parse_i8 127 ret", e_parse_i8 (&s, &iout, 0), num7);
	e_test_assert_ptr_eq ("e_parse_i8 127 ptr", s, &num7[3]);
	e_test_assert_eq ("e_parse_i8 127 out", i8, iout, 127);

	s = num8;
	e_test_assert_ptr_eq ("e_parse_i8 128 ret", e_parse_i8 (&s, &iout, 0), nullptr);
	e_test_assert_ptr_eq ("e_parse_i8 128 ptr", s, num8);

	s = num9;
	iout = 0;
	e_test_assert_ptr_eq ("e_parse_i8 -128 ret", e_parse_i8 (&s, &iout, 0), num9);
	e_test_assert_ptr_eq ("e_parse_i8 -128 ptr", s, &num9[4]);
	e_test_assert_eq ("e_parse_i8 -128 out", i8, iout, -128);

	s = num10;
	e_test_assert_ptr_eq ("e_parse_i8 -129 ret", e_parse_i8 (&s, &iout, 0), nullptr);
	e_test_assert_ptr_eq ("e_parse_i8 -129 ptr", s, num10);

	s = num11;
	e_test_assert_ptr_eq ("e_parse_i8 999999 ret", e_parse_i8 (&s, &iout, 0), nullptr);
	e_test_assert_ptr_eq ("e_parse_i8 999999 ptr", s, num11);

	s = num12;
	e_test_assert_ptr_eq ("e_parse_i8 -999999 ret", e_parse_i8 (&s, &iout, 0), nullptr);
	e_test_assert_ptr_eq ("e_parse_i8 -999999 ptr", s, num12);
}
