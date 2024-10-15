STDC ?= c23
MODE ?= debug

CC     := x86_64-w64-mingw32-gcc
AR     := x86_64-w64-mingw32-ar
CFLAGS := -std=$(STDC) -Iinc -I../convey/inc
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -pedantic
CFLAGS += -Wno-sign-conversion -Wno-attributes -Wno-stringop-truncation

ifeq ($(MODE),native)
CFLAGS += -DNDEBUG -O3 -march=native
else ifeq ($(MODE),release)
CFLAGS += -DNDEBUG -O3
else ifeq ($(MODE),release-safe)
CFLAGS += -DDEBUG -O3
else ifeq ($(MODE),debug)
CFLAGS += -DDEBUG -Og -ggdb3
else
$(error Invalid MODE `$(MODE)`, expected one of: native, release, release-safe, debug)
endif

IDENT := mingw-x64-$(STDC)-$(MODE)

LIB_SRC := $(wildcard src/*.c)
LIB_OBJ := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.o)
LIB_DEP := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.dep)
LIB_BIN := bin/$(IDENT)/empower.a
TEST_SRC := $(wildcard test/*.c)
TEST_OBJ := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.o)
TEST_DEP := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.dep)
TEST_BIN := bin/$(IDENT)/empower-test.exe

CONVEY_LIB := ../convey/bin/$(IDENT)/convey.a
CONVEY_MK  := make -C ../convey --file ../convey/build/mingw-x64.mk

MAKEFLAGS += --no-print-directory


all: $(LIB_BIN)

clean:
	rm -rf obj/ bin/
	$(CONVEY_MK) clean

test: $(TEST_BIN)
	@echo -e '\x1b[34mTEST\x1b[0m $(TEST_BIN)'
	@./$(TEST_BIN)

$(CONVEY_LIB):
	@$(CONVEY_MK) MODE=$(MODE) STDC=$(STDC)

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
