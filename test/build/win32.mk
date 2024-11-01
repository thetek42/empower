STDC ?= clatest

CC     := cl /nologo
CFLAGS := /std:$(STDC) /Isrc /D_CRT_SECURE_NO_WARNINGS
CFLAGS += /W4 /WX /permissive-
CFLAGS += /DDEBUG /Od /Zi

IDENT := win32-$(STDC)

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
	@$(CC) $(CFLAGS) /Fe:$@ $(TEST_SRC)
