# empower

Empower is a configurable C library consisting of a collection of modules that empower you to write
C applications faster and with more joy.

## Modules

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
| **e_math**     | Math-related functions   |
| **e_mem**      | Memory manipulation      |
| **e_parse**    | String parser utilities  |
| **e_result**   | Result type              |
| **e_str**      | Dynamically sized string |
| **e_test**     | Testing                  |
| **e_types**    | Common type aliases      |
| **e_vec**      | Vectors / dynamic arrays |

## Requirements

- A compiler that understands C99 or newer. Both GCC and Clang are tested.
- An operating system that conforms to POSIX.

Support for other compilers and operating systems may be added in the future.

## Usage

1. Download the library, e.g. to `lib/empower`
2. Optional: Modify the configuration file in `lib/empower/inc/empower_config.h`
3. Adapt your build system:
   - Build the library by calling `make -C lib/empower CC=[gcc|clang] STDC=[c99|c11|c23] MODE=[debug|release|release-safe]`
   - Add the include directory `lib/empower/inc` (usually done with `-I`)
   - Link your application with the static library in `lib/empower/bin/empower-<CC>-<STDC>-<MODE>.a`
4. Include `<empower.h>` in your code.
5. Have fun! Documentation for all functions and types can be found in the header files.

## Development

For running the tests, use `make test CC=[gcc|clang] STDC=[c99|c11|c23] MODE=[debug|release-safe]`

For generating a `compile_commands.json` that will allow for LSP support with clangd, download
[Bear](https://github.com/rizsotto/Bear) and run `bear -- make test CC=clang`.
