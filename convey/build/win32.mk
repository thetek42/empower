STDC ?= clatest
MODE ?= debug

CC     := cl /nologo
AR     := lib /nologo
CFLAGS := /std:$(STDC) /Iinc /D_CRT_SECURE_NO_WARNINGS
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

IDENT   := win32-$(STDC)-$(MODE)
LIB_HDR := inc/convey.h inc/c_win32.h
LIB_SRC := src/c_common.c src/c_win32.c
LIB_OBJ := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.obj)
LIB_BIN := bin/$(IDENT)/convey.lib


all: $(LIB_BIN)
	@:

clean:
	rm -rf obj/ bin/

.PHONY: all clean


$(LIB_BIN): $(LIB_OBJ)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mAR  \x1b[0m $(LIB_BIN)'
	@$(AR) /out:$@ $(LIB_OBJ)

obj/$(IDENT)/%.obj: src/%.c
	@mkdir -p $(@D)
	@echo -ne '\x1b[32mCC  \x1b[0m '
	@$(CC) $(CFLAGS) /c $< /Fo$@
