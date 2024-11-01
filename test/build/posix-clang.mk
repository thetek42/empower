STDC ?= c23

CC     := clang
CFLAGS := -std=$(STDC) -Isrc
CFLAGS += -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200809L
CFLAGS += -Wall -Wextra -Werror -Wdouble-promotion -Wconversion -pedantic
CFLAGS += -Wno-sign-conversion -Wno-unknown-attributes
CFLAGS += -DDEBUG -Og -ggdb3 -fsanitize=undefined,address,leak

IDENT := posix-clang-$(STDC)-$(MODE)

HEADERS := $(wildcard src/*.h)
TEST_SRC := $(wildcard test/*.c)
TEST_BIN := bin/$(IDENT)/empower-test


all: $(TEST_BIN)
	@echo -e '\x1b[34mTEST\x1b[0m $(TEST_BIN)'
	@./$(TEST_BIN)

clean:
	rm -rf obj/ bin/
	$(CONVEY_MK) clean

.PHONY: all clean


$(TEST_BIN): $(TEST_SRC) $(HEADERS)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mLINK\x1b[0m $(TEST_BIN)'
	@$(CC) $(CFLAGS) $(TEST_SRC) -o $@
