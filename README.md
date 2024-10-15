# empower

Empower is a configurable C library consisting of a collection of modules that empower you to write
C applications faster and with more joy.

## Modules

Empower consists of two parts:
- _Empower_ itself contains the main library code
- _Convey_ is a cross platform and compiler agnostic utility library

Empower contains the following modules:

| Module         | Description              |
| -------------- | ------------------------ |
| **e_alloc**    | Memory allocation        |
| **e_arena**    | Arena allocator          |
| **e_attrs**    | Attribute helpers        |
| **e_compat**   | C standard compatibility |
| **e_cstr**     | String utility functions |
| **e_debug**    | Debugging utilities      |
| **e_enc**      | Encoding and Encrypting  |
| **e_fs**       | File handling functions  |
| **e_ini**      | INI file parsing         |
| **e_log**      | Logging                  |
| **e_macro**    | Macro helpers            |
| **e_mem**      | Memory manipulation      |
| **e_parse**    | String parser utilities  |
| **e_rand**     | Randomization            |
| **e_result**   | Result type              |
| **e_str**      | Dynamically sized string |
| **e_test**     | Testing                  |
| **e_types**    | Common type aliases      |
| **e_vec**      | Vectors / dynamic arrays |

## Requirements

- **Supported C standards:** C99 and newer are supported. C23 is recommended.
- **Supported platforms:**
  - POSIX: Support for GCC and Clang. Tested on Linux with glibc.
  - Windows: Support for MinGW and MSVC. Tested with Wine, but _not_ on an actual Windows machine or
    VM. Thus, support may be a bit flaky.

## Usage

In the future, pre-built libraries will be provided, but for now you have to do these steps
manually.

1. Download the library, e.g. to `lib/empower`
2. Optional: Modify the configuration file in `lib/empower/empower/inc/empower_config.h`
3. Adapt your build system:
   - Build the library by calling `make -C lib/empower/empower --file lib/empower/empower/build/[SYSTEM].mk` (see [Building](#Building))
   - Add the include directories `lib/empower/empower/inc` and `lib/empower/convey/inc` (usually done with `-I`)
   - Link your application with the static library in `lib/empower/empower/bin/<SYSTEM>-<STDC>-<MODE>/empower.a` and `lib/empower/convey/bin/<SYSTEM>-<STDC>-<MODE>/convey.a`
4. Include `<empower.h>` in your code.
5. Have fun! Documentation for all functions and types can be found in the header files.

## Building

Makefiles for building the library are provided in `empower/build`. Choose one that fits the system
you are targeting. The currently supported systems are `posix-gcc`, `posix-clang`, `mingw-x64` and
`win32`. The `win32` Makefile probably won't work on Windows, but it does when executing `cl`
through Wine on Linux and since that's all I have, that's what you'll get as well :).

You can pass two options to the Makefiles:
- `MODE` tells the compiler in which mode to compile. Accepted values: `debug`, `release`, `release-safe`, `native`
- `STDC` is the C standard that should be use. Accepted values: `c99`, `c11`, `c17`, `c23`

## Development

For running the tests, use the `test` target of the Makefiles in `empower/build`.

For generating a `compile_commands.json` that will allow for LSP support with clangd, download
[Bear](https://github.com/rizsotto/Bear) and run the following commands:

```sh
make -C empower --file build/posix-clang.mk clean
bear -- make -C empower --file build/posix-clang.mk test
```

This will not work for MinGW and MSVC stuff. I might get around to implementing some kind of
workaround for that, but I don't care enough about Windows to justify the time investment.
