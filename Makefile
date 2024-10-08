# TODO: this makefile is a mess.

AR ?= ar
CC ?= gcc
STDC ?= c23
MODE ?= debug

ifeq ($(CC),cc)
CC := gcc
endif

ifeq ($(CC),gcc)

CFLAGS := -std=$(STDC) -Iinc
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -Wno-sign-conversion -Wno-attributes -Wno-stringop-truncation -pedantic
CFLAGS_DEP = -MD -MP -MF $(@:%.o=%.dep)

ifeq ($(MODE),release)
CFLAGS += -DNDEBUG -O3 -march=native
else ifeq ($(MODE),release-safe)
CFLAGS += -DNDEBUG -O3 -march=native -fsanitize=undefined,address,leak
else
CFLAGS += -DDEBUG -Og -ggdb3 -fsanitize=undefined,address,leak
endif

else ifeq ($(CC),clang)

CFLAGS := -std=$(STDC) -Iinc
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -Wno-sign-conversion -Wno-unknown-attributes -pedantic
CFLAGS_DEP = -MD -MP -MF $(@:%.o=%.dep)

ifeq ($(MODE),release)
CFLAGS += -DNDEBUG -O3 -march=native
else ifeq ($(MODE),release-safe)
CFLAGS += -DNDEBUG -O3 -march=native -fsanitize=undefined,address,leak
else
CFLAGS += -DDEBUG -Og -ggdb3 -fsanitize=undefined,address,leak
endif

else
# TODO: support other compilers (mostly msvc i guess?)
endif

IDENT := $(CC)-$(STDC)-$(MODE)

LIB_SRC := $(wildcard src/*.c)
LIB_OBJ := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.o)
LIB_DEP := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.dep)
LIB_BIN := bin/$(IDENT)/empower.a
TEST_SRC := $(wildcard test/*.c)
TEST_OBJ := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.o)
TEST_DEP := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.dep)
TEST_BIN := bin/$(IDENT)/empower-test


all: $(LIB_BIN)

clean:
	rm -rf obj/ bin/

test: $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: all clean test


$(LIB_BIN): $(LIB_OBJ)
	@mkdir -p $(@D)
	$(AR) rcs $@ $(LIB_OBJ)

$(TEST_BIN): $(TEST_OBJ) $(LIB_BIN)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(LIB_BIN) -o $@

obj/$(IDENT)/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS_DEP)

obj/test-$(IDENT)/%.o: test/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS_DEP)


-include $(LIB_DEP)
-include $(TEST_DEP)
