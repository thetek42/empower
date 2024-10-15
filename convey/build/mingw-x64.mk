STDC ?= c23
MODE ?= debug

CC     := x86_64-w64-mingw32-gcc
AR     := x86_64-w64-mingw32-ar
CFLAGS := -std=$(STDC) -Iinc
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

IDENT   := mingw-x64-$(STDC)-$(MODE)
LIB_HDR := inc/convey.h inc/c_mingw.h
LIB_SRC := src/c_common.c src/c_mingw.c
LIB_OBJ := $(LIB_SRC:src/%.c=obj/$(IDENT)/%.o)
LIB_BIN := bin/$(IDENT)/convey.a


all: $(LIB_BIN)
	@:

clean:
	rm -rf obj/ bin/

.PHONY: all clean


$(LIB_BIN): $(LIB_OBJ)
	@mkdir -p $(@D)
	@echo -e '\x1b[36mAR  \x1b[0m $(LIB_BIN)'
	@$(AR) rcs $@ $(LIB_OBJ)

obj/$(IDENT)/%.o: src/%.c
	@mkdir -p $(@D)
	@echo -e '\x1b[32mCC  \x1b[0m $<'
	@$(CC) $(CFLAGS) -c $< -o $@ $(CFLAGS_DEP)
