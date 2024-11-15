# empower

Empower is a collection of header-only libraries for C that empower you to write applications faster
and with more joy.

## Modules

Empower contains the following modules:

| Module         | Description              |
| -------------- | ------------------------ |
| **e_alloc**    | Memory allocation        |
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

- **Supported C standards:** C99 and newer are supported, C23 is recommended.
- **Supported platforms:**
  - POSIX: Support for GCC and Clang. Tested on Linux with glibc.
  - Windows: Support for MinGW and MSVC. Tested with Wine, but _not_ on an actual Windows machine or
    VM. Thus, support may be a bit flaky.

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

For generating a `compile_commands.json` that will allow for LSP support with clangd, download
[Bear](https://github.com/rizsotto/Bear) and run the following commands:

```sh
make --file test/build/posix-clang.mk clean
bear -- make --file test/build/posix-clang.mk
```

This will not work for MinGW and MSVC stuff. I might get around to implementing some kind of
workaround for that, but I don't care enough about Windows to justify the time investment.
