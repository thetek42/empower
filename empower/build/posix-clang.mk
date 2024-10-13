STDC ?= c23
MODE ?= debug

AR     := ar
CC     := clang
CFLAGS := -std=$(STDC) -Iinc -I../convey/inc
CFLAGS += -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -pedantic
CFLAGS += -Wno-sign-conversion -Wno-unknown-attributes
CFLAGS_DEP =

ifeq ($(MODE),native)
CFLAGS += -DNDEBUG -O3 -march=native
else ifeq ($(MODE),release)
CFLAGS += -DNDEBUG -O3
else ifeq ($(MODE),release-safe)
CFLAGS += -DDEBUG -O3 -fsanitize=undefined,address,leak
else ifeq ($(MODE),debug)
CFLAGS += -DDEBUG -Og -ggdb3 -fsanitize=undefined,address,leak
else
$(error Invalid MODE `$(MODE)`, expected one of: native, release, release-safe, debug)
endif

IDENT := posix-clang-$(STDC)-$(MODE)

LIB_SRC := $(wildcard src/*.c)
LIB_OBJ := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.o)
LIB_DEP := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.dep)
LIB_BIN := bin/$(IDENT)/empower.a
TEST_SRC := $(wildcard test/*.c)
TEST_OBJ := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.o)
TEST_DEP := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.dep)
TEST_BIN := bin/$(IDENT)/empower-test

CONVEY_LIB := ../convey/bin/$(IDENT)/convey.a

MAKEFLAGS += --no-print-directory


all: $(LIB_BIN)

clean:
	rm -rf obj/ bin/
	make -C ../convey --file ../convey/build/posix-clang.mk clean

test: $(TEST_BIN)
	@echo -e '\x1b[34mTEST\x1b[0m $(TEST_BIN)'
	@./$(TEST_BIN)

$(CONVEY_LIB):
	@make -C ../convey --file ../convey/build/posix-clang.mk MODE=$(MODE) STDC=$(STDC)

.PHONY: all clean test $(CONVEY_LIB)


$(LIB_BIN): $(LIB_OBJ) $(CONVEY_LIB)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mAR  \x1b[0m $(LIB_BIN)'
	@$(AR) rcs $@ $(LIB_OBJ)

$(TEST_BIN): $(TEST_OBJ) $(LIB_BIN)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mLINK\x1b[0m $(TEST_BIN)'
	@$(CC) $(CFLAGS) $(TEST_OBJ) $(LIB_BIN) $(CONVEY_LIB) -o $@

obj/$(IDENT)/%.o: src/%.c
	@mkdir -p $(@D)
	@echo -e '\x1b[32mCC  \x1b[0m $<'
	@$(CC) $(CFLAGS) -c $< -o $@ -MD -MP -MF $(@:%.o=%.dep)

obj/test-$(IDENT)/%.o: test/%.c
	@mkdir -p $(@D)
	@echo -e '\x1b[32mCC  \x1b[0m $<'
	@$(CC) $(CFLAGS) -c $< -o $@ -MD -MP -MF $(@:%.o=%.dep)


-include $(LIB_DEP)
-include $(TEST_DEP)
