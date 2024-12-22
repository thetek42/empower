#include <e_macro.h>

#include <e_test.h>
#include <e_types.h>

typedef struct {
	u32 a;
	u16 b;
	u8 c;
	u64 d;
} Foo;

void
test_macro (void)
{
	Foo foo;
	u8 *field;
	u32 arr[64];

	foo = (Foo) {0};
	field = &foo.c;
	e_test_assert_ptr_eq ("E_CONTAINER_OF", E_CONTAINER_OF (field, Foo, c), &foo);

	e_test_assert_eq ("E_ARRAY_SIZE", usize, E_ARRAY_SIZE (arr), 64);
}
