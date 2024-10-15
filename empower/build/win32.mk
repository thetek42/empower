STDC ?= clatest
MODE ?= debug

CC     := cl /nologo
AR     := lib /nologo
CFLAGS := /std:$(STDC) /Iinc /I../convey/inc /D_CRT_SECURE_NO_WARNINGS
CFLAGS += /W4 /WX /permissive-

ifeq ($(MODE),native)
CFLAGS += /DNDEBUG /Ox
else ifeq ($(MODE),release)
CFLAGS += /DNDEBUG /Ox
else ifeq ($(MODE),release-safe)
CFLAGS += /DDEBUG /Ox
else ifeq ($(MODE),debug)
CFLAGS += /DDEBUG /Od /Zi
else
$(error Invalid MODE `$(MODE)`, expected one of: native, release, release-safe, debug)
endif

IDENT := win32-$(STDC)-$(MODE)

LIB_SRC := $(wildcard src/*.c)
LIB_OBJ := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.obj)
LIB_BIN := bin/$(IDENT)/empower.lib
TEST_SRC := $(wildcard test/*.c)
TEST_OBJ := $(TEST_SRC:test/%.c=obj/test-$(IDENT)/%.obj)
TEST_BIN := bin/$(IDENT)/empower-test.exe

CONVEY_LIB := ../convey/bin/$(IDENT)/convey.lib
CONVEY_MK  := make -C ../convey --file ../convey/build/win32.mk

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
	@$(AR) /out:$@ $(LIB_OBJ)

$(TEST_BIN): $(TEST_OBJ) $(LIB_BIN)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mLINK\x1b[0m $(TEST_BIN)'
	@$(CC) $(CFLAGS) /Fe:$@ $(TEST_OBJ) $(LIB_BIN) $(CONVEY_LIB)

obj/$(IDENT)/%.obj: src/%.c
	@mkdir -p $(@D)
	@echo -ne '\x1b[32mCC  \x1b[0m '
	@$(CC) $(CFLAGS) /c $< /Fo$@

obj/test-$(IDENT)/%.obj: test/%.c
	@mkdir -p $(@D)
	@echo -ne '\x1b[32mCC  \x1b[0m '
	@$(CC) $(CFLAGS) /c $< /Fo$@


-include $(LIB_DEP)
-include $(TEST_DEP)
