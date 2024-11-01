STDC ?= c2x
MODE ?= debug

CC     := x86_64-w64-mingw32-gcc
CFLAGS := -std=$(STDC) -Isrc
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -pedantic
CFLAGS += -Wno-sign-conversion -Wno-attributes -Wno-stringop-truncation
CFLAGS += -DDEBUG -Og -ggdb3

IDENT := mingw-x64-$(STDC)

HEADERS := $(wildcard src/*.h)
TEST_SRC := $(wildcard test/*.c)
TEST_BIN := bin/$(IDENT)/empower-test.exe


all: $(TEST_BIN)
	@echo -e '\x1b[34mTEST \x1b[0m $(TEST_BIN)'
	@./$(TEST_BIN)

clean:
	rm -rf bin/$(IDENT)

.PHONY: all clean


$(TEST_BIN): $(TEST_SRC) $(HEADERS)
	@mkdir -p $(@D)
	@echo -e '\x1b[32mCC   \x1b[0m $(TEST_BIN)'
	@$(CC) $(CFLAGS) $(TEST_SRC) -o $@
