# empower

Empower is a configurable C library consisting of a collection of modules that empower you to write
C applications faster and with more joy.

## Modules

| Module         | Description                    |
| -------------- | ------------------------------ |
| **e_alloc**    | Memory allocation              |
| **e_arena**    | Arena allocator                |
| **e_cstr**     | String utility functions       |
| **e_debug**    | Debugging utilities            |
| **e_enc**      | Encoding and Encrypting        |
| **e_fs**       | File handling functions        |
| **e_ini**      | INI file parsing               |
| **e_log**      | Logging                        |
| **e_macro**    | Macro helpers                  |
| **e_math**     | Math-related functions         |
| **e_result**   | Result type                    |
| **e_str**      | Dynamically sized string       |
| **e_test**     | Testing                        |
| **e_types**    | Common type aliases            |
| **e_vec**      | Vectors / dynamic arrays       |

## Requirements

- A compiler that understands (at least some parts of) C23. Both GCC and Clang are tested.
- An operating system that conforms to POSIX.

Both of these things will not be required anymore in the future; I was simply too lazy to make this
library portable in its infancy stage.

## Usage

1. Download the library, e.g. to `lib/empower`
2. Optional: Modify the configuration file in `lib/empower/inc/empower-config.h`
3. Optional: Modify `lib/empower/Makefile` if you need a different compiler or different flags
4. Adapt your build system:
   - Build the libraries by calling `make -C lib/empower [debug|release]`
   - Add the include directory `lib/empower/inc`
   - Make sure that you use C23 as the C standard (requirement will be removed in the future)
   - Link your application with the static library in `lib/empower/bin/empower[-debug].a`
5. Include `<empower.h>` in your code.
6. Have fun! Documentation for all functions and types can be found in the header files.
