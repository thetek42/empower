# empower

Empower is a collection of header-only libraries for C that empower you to write applications faster
and with more joy.

## Modules

Empower contains the following modules:

| Module         | Description              |
| -------------- | ------------------------ |
| **e_alloc**    | Memory allocation        |
| **e_arena**    | Arena allocator          |
| **e_base64**   | Base64 encoding/decoding |
| **e_cobs**     | COBS encoding/decoding   |
| **e_cobsr**    | COBS/R encoding/decoding |
| **e_cstr**     | String utility functions |
| **e_debug**    | Debugging utilities      |
| **e_fs**       | File handling functions  |
| **e_ini**      | INI file parsing         |
| **e_log**      | Logging                  |
| **e_macro**    | Macro helpers            |
| **e_mem**      | Memory manipulation      |
| **e_parse**    | String parser utilities  |
| **e_rand**     | Randomization            |
| **e_rbuf**     | Ring Buffer              |
| **e_result**   | Result type              |
| **e_str**      | Dynamically sized string |
| **e_test**     | Testing                  |
| **e_types**    | Common type aliases      |
| **e_vec**      | Vectors / dynamic arrays |

## Requirements

- **Supported C standards:**
  - C99 or newer is required (C89 is not supported).
  - However, it is recommended to always use the most recent C version (e.g. C23).
- **Supported compilers:**
  - GCC, Clang, MinGW GCC and MSVC are officially supported.
  - Others likely work as well.
- **Supported platforms:**
  - Many modules are platform independent. Thus, they are also suitable for embedded systems.
  - For platform dependent modules (e.g. e\_fs):
    - POSIX: Fully supported. Tested on Linux with glibc.
    - Windows: Support for MinGW and "native" Windows builds. Tested with Wine, but _not_ on an
      actual Windows machine or VM. Thus, support may be a bit flaky. Pull requests welcome.

## Usage

The process is the same as for all STB-style header-only libraries:

1. Download the `.h` file for the libraries you need and place them somewhere in your project.
2. Include the libraries in the files where you need them.
3. In one `.c` file, you will have to provide the implementation for the functions. For instance,
   when using `e_cstr`, you have to `#define E_CSTR_IMPL` before the include. Not all modules
   require this; refer to the module documentation.
4. Have fun!

## Development

For running the tests, use the Makefiles in `test/build`, for example:

```sh
make --file test/build/posix-gcc.mk
make --file test/build/posix-clang.mk STDC=c99
```

For generating a `compile_commands.json` that will allow for LSP support with clangd, run the
`gen_compile_commands.py` script as follows:

```sh
python gen_compile_commands.py > compile_commands.json
```

This will not work for MinGW and MSVC stuff. I might get around to implementing some kind of
workaround for that, but I don't care enough about Windows to justify the time investment.
