#include <e_macro.h>

#include <e_test.h>
#include <e_types.h>

#define ORIENTATION_ENUM(X)  \
	X(Orientation_North) \
	X(Orientation_East)  \
	X(Orientation_South) \
	X(Orientation_West)
E_MACRO_DECL_STRINGIFY_ENUM (ORIENTATION_ENUM, Orientation, orientation_to_str);
E_MACRO_IMPL_STRINGIFY_ENUM (ORIENTATION_ENUM, Orientation, orientation_to_str)

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

	e_test_assert_str_eq ("E_MACRO_STRINGIFY_ENUM", orientation_to_str (Orientation_South), "Orientation_South");
}
