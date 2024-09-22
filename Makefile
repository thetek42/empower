CC := cc
AR := ar

CFLAGS := -std=c23 -Iinc
CFLAGS_WARN := -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -Wno-sign-conversion -Wno-attributes -pedantic
CFLAGS_DEBUG := -DDEBUG -Og -ggdb3 -fsanitize=undefined,address,leak
CFLAGS_RELEASE := -DNDEBUG -O3 -march=native

SRC_FILES := $(wildcard src/*.c)
OBJ_DEBUG := $(SRC_FILES:src/%.c=obj/%-debug.o)
OBJ_RELEASE := $(SRC_FILES:src/%.c=obj/%-release.o)
DEP_FILES := $(SRC_FILES:src/%.c=obj/%.dep)
LIB_DEBUG := bin/empower-debug.a
LIB_RELEASE := bin/empower.a
TEST_FILES := $(wildcard test/test.c)
TEST_TARGET := bin/empower-test


all: $(LIB_DEBUG) $(LIB_RELEASE)

clean:
	rm -rf obj/ bin/

debug: $(LIB_DEBUG)

release: $(LIB_RELEASE)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

.PHONY: all clean test


$(LIB_DEBUG): $(OBJ_DEBUG)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(OBJ_DEBUG)

$(LIB_RELEASE): $(OBJ_RELEASE)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(OBJ_RELEASE)

$(TEST_TARGET): $(TEST_FILES) $(LIB_DEBUG)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_WARN) $(CFLAGS_DEBUG) $(TEST_FILES) $(LIB_DEBUG) -o $@ -MD -MP -MF obj/test.dep

obj/%-debug.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_WARN) $(CFLAGS_DEBUG) -c $< -o $@ -MD -MP -MF $(<:src/%.c=obj/%.dep)

obj/%-release.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CFLAGS_WARN) $(CFLAGS_RELEASE) -c $< -o $@ -MD -MP -MF $(<:src/%.c=obj/%.dep)


-include $(DEP_FILES)
