#include <e_vec.h>

#include <e_test.h>
#include <e_types.h>

E_VEC_DECL (int, E_Vec_Int, e_vec_int);
E_VEC_IMPL (int, E_Vec_Int, e_vec_int);

static int int_comp_func (const int *a, const int *b);

void
test_vec (void)
{
	E_Vec_Int vec, vec_clone;
	int slice[] = { 4, 5, 6, 7, 8 };
	int slice2[] = { 3, 4 };
	int slice3[] = { 4, 3 };
	int slice4[] = { 2, 3 };
	int slice5[] = { 4, 4 };
	int key, *popped;
	const int *res;
	usize len;

	/* [] */
	vec = e_vec_int_init ();
	e_test_assert_null ("e_vec_init ptr", vec.ptr);
	e_test_assert_eq ("e_vec_init len", usize, vec.len, 0);
	e_test_assert_eq ("e_vec_init cap", usize, vec.cap, 0);
	e_test_assert_null ("e_vec_first empty", e_vec_int_get_first (&vec));
	e_test_assert_null ("e_vec_last empty", e_vec_int_get_last (&vec));

	/* [1] */
	e_vec_int_append (&vec, 1);
	e_test_assert_eq ("e_vec_append 1 ptr[0]", int, vec.ptr[0], 1);
	e_test_assert_eq ("e_vec_append 1 len", usize, vec.len, 1);
	e_test_assert_eq ("e_vec_append 1 cap", usize, vec.cap, 1);

	/* [1,2] */
	e_vec_int_append (&vec, 2);
	e_test_assert_eq ("e_vec_append 2 ptr[1]", int, vec.ptr[1], 2);
	e_test_assert_eq ("e_vec_append 2 len", usize, vec.len, 2);
	e_test_assert_eq ("e_vec_append 2 cap", usize, vec.cap, 2);

	/* [1,2,3] */
	e_vec_int_append (&vec, 3);
	e_test_assert_eq ("e_vec_append 3 ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_append 3 len", usize, vec.len, 3);
	e_test_assert_eq ("e_vec_append 3 cap", usize, vec.cap, 4);

	/* [1,2,3,4,5,6,7,8] */
	e_vec_int_append_slice (&vec, slice, 5);
	e_test_assert_eq ("e_vec_append_slice ptr[3]", int, vec.ptr[3], 4);
	e_test_assert_eq ("e_vec_append_slice ptr[7]", int, vec.ptr[7], 8);
	e_test_assert_eq ("e_vec_append_slice len", usize, vec.len, 8);
	e_test_assert_eq ("e_vec_append_slice cap", usize, vec.cap, 8);

	e_test_assert_ptr_eq ("e_vec_find existing", e_vec_int_find (&vec, 4), &vec.ptr[3]);
	e_test_assert_null ("e_vec_find nonexisting", e_vec_int_find (&vec, 42));
	e_test_assert_eq ("e_vec_find_idx existing", isize, e_vec_int_find_idx (&vec, 4), 3);
	e_test_assert_eq ("e_vec_find_idx nonexisting", isize, e_vec_int_find_idx (&vec, 42), -1);
	e_test_assert_ptr_eq ("e_vec_find_slice existing", e_vec_int_find_slice (&vec, slice2, 2), &vec.ptr[2]);
	e_test_assert_null ("e_vec_find_slice nonexisting", e_vec_int_find_slice (&vec, slice3, 2));
	e_test_assert_eq ("e_vec_find_slice_idx existing", isize, e_vec_int_find_slice_idx (&vec, slice2, 2), 2);
	e_test_assert_eq ("e_vec_find_slice_idx nonexisting", isize, e_vec_int_find_slice_idx (&vec, slice3, 2), -1);
	e_test_assert ("e_vec_contains true", e_vec_int_contains (&vec, 4));
	e_test_assert ("e_vec_contains false", !e_vec_int_contains (&vec, 42));
	e_test_assert ("e_vec_contains_slice true", e_vec_int_contains_slice (&vec, slice2, 2));
	e_test_assert ("e_vec_contains_slice false", !e_vec_int_contains_slice (&vec, slice3, 2));
	e_test_assert_ptr_eq ("e_vec_get existing", e_vec_int_get (&vec, 4), &vec.ptr[4]);
	e_test_assert_null ("e_vec_get nonexisting", e_vec_int_get (&vec, 42));
	e_test_assert_ptr_eq ("e_vec_get_back existing", e_vec_int_get_back (&vec, 2), &vec.ptr[5]);
	e_test_assert_null ("e_vec_get_back nonexisting", e_vec_int_get_back (&vec, 42));
	e_test_assert_ptr_eq ("e_vec_first", e_vec_int_get_first (&vec), &vec.ptr[0]);
	e_test_assert_ptr_eq ("e_vec_last", e_vec_int_get_last (&vec), &vec.ptr[7]);
	e_test_assert ("e_vec_set ok", e_vec_int_set (&vec, 1, 42));
	e_test_assert ("e_vec_set out of range", !e_vec_int_set (&vec, 42, 1));
	e_test_assert ("e_vec_set_slice ok", e_vec_int_set_slice (&vec, 1, slice4, 2));
	e_test_assert ("e_vec_set_slice out of range", !e_vec_int_set_slice (&vec, 42, slice4, 2));

	/* [1,2,3,4,5,6,7] */
	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 1 item", int, *popped, 8);
	e_test_assert_eq ("e_vec_pop 1 len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_pop 1 cap", usize, vec.cap, 8);

	/* [1,2,3,4,5,6] */
	e_vec_int_pop (&vec, &popped);
	e_test_assert_eq ("e_vec_pop 2 item", int, *popped, 7);
	e_test_assert_eq ("e_vec_pop 2 len", usize, vec.len, 6);
	e_test_assert_eq ("e_vec_pop 2 cap", usize, vec.cap, 8);

	/* [1,2,3,4,5,6,9] */
	e_vec_int_append (&vec, 9);
	e_test_assert_eq ("e_vec_append 4 ptr[6]", int, vec.ptr[6], 9);
	e_test_assert_eq ("e_vec_append 4 len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_append 4 cap", usize, vec.cap, 8);

	/* [1,2,3,11,4,5,6,9] */
	e_vec_int_insert (&vec, 3, 11);
	e_test_assert_eq ("e_vec_insert ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_insert ptr[3]", int, vec.ptr[3], 11);
	e_test_assert_eq ("e_vec_insert ptr[4]", int, vec.ptr[4], 4);
	e_test_assert_eq ("e_vec_insert len", usize, vec.len, 8);
	e_test_assert_eq ("e_vec_insert cap", usize, vec.cap, 8);

	/* [1,2,3,11,4,5,6,9,12] */
	e_vec_int_insert (&vec, 42, 12);
	e_test_assert_eq ("e_vec_insert append ptr[7]", int, vec.ptr[7], 9);
	e_test_assert_eq ("e_vec_insert append ptr[8]", int, vec.ptr[8], 12);
	e_test_assert_eq ("e_vec_insert append len", usize, vec.len, 9);
	e_test_assert_eq ("e_vec_insert append cap", usize, vec.cap, 16);

	/* [1,2,3,11,4,5,3,4,6,9,12] */
	e_vec_int_insert_slice (&vec, 6, slice2, 2);
	e_test_assert_eq ("e_vec_insert_slice ptr[5]", int, vec.ptr[5], 5);
	e_test_assert_eq ("e_vec_insert_slice ptr[6]", int, vec.ptr[6], 3);
	e_test_assert_eq ("e_vec_insert_slice ptr[7]", int, vec.ptr[7], 4);
	e_test_assert_eq ("e_vec_insert_slice ptr[8]", int, vec.ptr[8], 6);
	e_test_assert_eq ("e_vec_insert_slice len", usize, vec.len, 11);
	e_test_assert_eq ("e_vec_insert_slice cap", usize, vec.cap, 16);

	/* [1,2,3,11,4,5,3,4,6,9,12,3,4] */
	e_vec_int_insert_slice (&vec, 42, slice2, 2);
	e_test_assert_eq ("e_vec_insert_slice append ptr[10]", int, vec.ptr[10], 12);
	e_test_assert_eq ("e_vec_insert_slice append ptr[11]", int, vec.ptr[11], 3);
	e_test_assert_eq ("e_vec_insert_slice append ptr[12]", int, vec.ptr[12], 4);
	e_test_assert_eq ("e_vec_insert_slice append len", usize, vec.len, 13);
	e_test_assert_eq ("e_vec_insert_slice append cap", usize, vec.cap, 16);

	vec_clone = e_vec_int_clone (&vec);

	e_vec_int_append (&vec, 4);
	e_vec_int_append (&vec, 4);
	e_test_assert_eq ("e_vec_count", usize, e_vec_int_count (&vec, 3), 3);
	e_test_assert_eq ("e_vec_count_slice 1", usize, e_vec_int_count_slice (&vec, slice2, 2), 2);
	e_test_assert_eq ("e_vec_count_slice 2", usize, e_vec_int_count_slice (&vec, slice5, 2), 1);
	e_test_assert_eq ("e_vec_count_slice_overlap", usize, e_vec_int_count_slice_overlap (&vec, slice5, 2), 2);
	e_vec_int_pop_slice (&vec, nullptr, 2);

	/* [1,2,3,11,4,5,3,4,6] */
	len = e_vec_int_pop_slice (&vec, &popped, 4);
	e_test_assert_eq ("e_vec_pop_slice 1 item", int, *popped, 9);
	e_test_assert_eq ("e_vec_pop_slice 1 len", usize, vec.len, 9);
	e_test_assert_eq ("e_vec_pop_slice 1 cap", usize, vec.cap, 16);
	e_test_assert_eq ("e_vec_pop_slice 1 returned len", usize, len, 4);

	/* [1,2,3,11,4,5,3] */
	len = e_vec_int_remove (&vec, 7, 3);
	e_test_assert_eq ("e_vec_remove end ptr[6]", int, vec.ptr[6], 3);
	e_test_assert_eq ("e_vec_remove end len", usize, vec.len, 7);
	e_test_assert_eq ("e_vec_remove end cap", usize, vec.cap, 16);
	e_test_assert_eq ("e_vec_remove end ret", usize, len, 2);

	/* [1,2,3,5,3] */
	e_vec_int_remove (&vec, 3, 2);
	e_test_assert_eq ("e_vec_remove middle ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_remove middle ptr[3]", int, vec.ptr[3], 5);
	e_test_assert_eq ("e_vec_remove middle len", usize, vec.len, 5);
	e_test_assert_eq ("e_vec_remove middle cap", usize, vec.cap, 16);

	/* [3,5,3,2,1] */
	e_vec_int_reverse (&vec);
	e_test_assert_eq ("e_vec_reverse ptr[0]", int, vec.ptr[0], 3);
	e_test_assert_eq ("e_vec_reverse ptr[1]", int, vec.ptr[1], 5);
	e_test_assert_eq ("e_vec_reverse ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_reverse ptr[3]", int, vec.ptr[3], 2);
	e_test_assert_eq ("e_vec_reverse ptr[4]", int, vec.ptr[4], 1);

	/* [] */
	len = e_vec_int_pop_slice (&vec, &popped, 8);
	e_test_assert_eq ("e_vec_pop_slice 2 item", int, *popped, 3);
	e_test_assert_eq ("e_vec_pop_slice 2 len", usize, vec.len, 0);
	e_test_assert_eq ("e_vec_pop_slice 2 cap", usize, vec.cap, 16);
	e_test_assert_eq ("e_vec_pop_slice 2 returned len", usize, len, 5);

	e_vec_int_deinit (&vec);

	vec = e_vec_int_repeat (42, 3);
	e_test_assert_eq ("e_vec_repeat ptr[0]", int, vec.ptr[0], 42);
	e_test_assert_eq ("e_vec_repeat ptr[1]", int, vec.ptr[1], 42);
	e_test_assert_eq ("e_vec_repeat ptr[2]", int, vec.ptr[2], 42);
	e_test_assert_eq ("e_vec_repeat len", usize, vec.len, 3);
	e_vec_int_deinit (&vec);

	vec = e_vec_int_repeat_slice (slice2, 2, 2);
	e_test_assert_eq ("e_vec_repeat ptr[0]", int, vec.ptr[0], 3);
	e_test_assert_eq ("e_vec_repeat ptr[1]", int, vec.ptr[1], 4);
	e_test_assert_eq ("e_vec_repeat ptr[2]", int, vec.ptr[2], 3);
	e_test_assert_eq ("e_vec_repeat ptr[3]", int, vec.ptr[3], 4);
	e_test_assert_eq ("e_vec_repeat len", usize, vec.len, 4);
	e_vec_int_deinit (&vec);

	/* [4,3,12,9,6,4,3,5,4,11,3,2,1] */
	e_vec_int_reverse (&vec_clone);

	/* [1,2,3,3,3,4,4,4,5,6,9,11,12] */
	e_vec_int_sort (&vec_clone, int_comp_func);
	e_test_assert_eq ("e_vec_sort ptr[0]", int, vec_clone.ptr[0], 1);
	e_test_assert_eq ("e_vec_sort ptr[1]", int, vec_clone.ptr[1], 2);
	e_test_assert_eq ("e_vec_sort ptr[2]", int, vec_clone.ptr[2], 3);
	e_test_assert_eq ("e_vec_sort ptr[3]", int, vec_clone.ptr[3], 3);
	e_test_assert_eq ("e_vec_sort ptr[4]", int, vec_clone.ptr[4], 3);
	e_test_assert_eq ("e_vec_sort ptr[5]", int, vec_clone.ptr[5], 4);
	e_test_assert_eq ("e_vec_sort ptr[6]", int, vec_clone.ptr[6], 4);
	e_test_assert_eq ("e_vec_sort ptr[7]", int, vec_clone.ptr[7], 4);
	e_test_assert_eq ("e_vec_sort ptr[8]", int, vec_clone.ptr[8], 5);
	e_test_assert_eq ("e_vec_sort ptr[9]", int, vec_clone.ptr[9], 6);
	e_test_assert_eq ("e_vec_sort ptr[10]", int, vec_clone.ptr[10], 9);
	e_test_assert_eq ("e_vec_sort ptr[11]", int, vec_clone.ptr[11], 11);
	e_test_assert_eq ("e_vec_sort ptr[12]", int, vec_clone.ptr[12], 12);

	key = 6;
	res = e_vec_int_bsearch (&vec_clone, &key, int_comp_func);
	e_test_assert_ptr_eq ("e_vec_bsearch found", res, &vec_clone.ptr[9]);
	key = 42;
	res = e_vec_int_bsearch (&vec_clone, &key, int_comp_func);
	e_test_assert_null ("e_vec_bsearch not found", res);

	e_vec_int_deinit (&vec_clone);
}

static int
int_comp_func (const int *a, const int *b)
{
	return *a - *b;
}
