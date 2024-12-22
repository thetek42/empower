STDC ?= c2x
MODE ?= debug

CC     := x86_64-w64-mingw32-gcc
CFLAGS := -std=$(STDC) -Isrc
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -pedantic
CFLAGS += -Wno-sign-conversion -Wno-attributes -Wno-stringop-truncation
CFLAGS += -DDEBUG -Og -ggdb3

IDENT := mingw-x64-$(STDC)

TEST_SRC := $(wildcard test/*.c)
TEST_OBJ := $(TEST_SRC:test/%.c=obj/$(IDENT)/%.o)
TEST_DEP := $(TEST_SRC:test/%.c=obj/$(IDENT)/%.dep)
TEST_BIN := bin/$(IDENT)/empower-test.exe


all: $(TEST_BIN)
	@echo -e '\x1b[34mTEST \x1b[0m $(TEST_BIN)'
	@./$(TEST_BIN)

clean:
	rm -rf bin/$(IDENT) obj/$(IDENT)

.PHONY: all clean


$(TEST_BIN): $(TEST_OBJ)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mLINK \x1b[0m $(TEST_BIN)'
	@$(CC) $(CFLAGS) $(TEST_OBJ) -o $@

obj/$(IDENT)/%.o: test/%.c
	@mkdir -p $(@D)
	@echo -e '\x1b[32mCC   \x1b[0m $<'
	@$(CC) $(CFLAGS) -c $< -o $@ -MD -MP -MF $(<:test/%.c=obj/$(IDENT)/%.dep)


-include $(TEST_DEP)
