# Empower

Empower is a collection of header-only libraries for C that empower you to write
applications faster and with more joy.

## Modules

Empower contains the following modules:

| Module         | Description                 |
| -------------- | --------------------------- |
| **e_alloc**    | Memory allocation           |
| **e_arena**    | Arena allocator             |
| **e_base64**   | Base64 encoding/decoding    |
| **e_bitvec**   | Bit array                   |
| **e_cobs**     | COBS encoding/decoding      |
| **e_cobsr**    | COBS/R encoding/decoding    |
| **e_cstr**     | String utility functions    |
| **e_debug**    | Debugging utilities         |
| **e_ini**      | INI file parsing            |
| **e_log**      | Logging                     |
| **e_macro**    | Macro helpers               |
| **e_mem**      | Memory manipulation         |
| **e_rand**     | Randomization               |
| **e_stdc**     | Uniform feature test macros |
| **e_test**     | Testing                     |

## Requirements

Check [SUPPORT.md](SUPPORT.md) for detailed information about which C standards,
compilers and platforms are supported.

TL;DR:
- **C Standard Version:** Most modules support C89, some require C99, but C11
  and C23 unlock a few extra features.
- **Compilers:** GCC, Clang, TinyCC, MinGW GCC and MSVC are officially supported,
  others likely work as well.
- **Platforms:** Most modules are platform independent and work in freestanding
  environments. Platform-dependent modules work on POSIX (tested on Linux with
  glibc) and Windows (tested with Wine with both MinGW and MSVC).

## Usage

The process is the same as for all STB-style header-only libraries:

1. Download the `.h` file for the libraries you need and place them somewhere in
   your project.
2. Include the libraries in the files where you need them.
3. In one `.c` file, you will have to provide the implementation for the
   functions. For instance, when using `e_mem`, you have to `#define E_MEM_IMPL`
   before the include.
4. Have fun!

## Testing

To execute the tests, you can use the `tooling/tests.py` script:

```sh
# execute tests for all targets and C standards
python3 tooling/tests.py
# execute tests for targets "native-gcc" and "native-clang" for all C standards
python3 tooling/tests.py native-gcc native-clang
# execute tests for all targets with C11
python3 tooling/tests.py c11
# execute tests for "mingw-x64" with C89 and C99
python3 tooling/tests.py mingw-x64 c89 c99
```

The targets for the tests are defined in `tooling/targets.py`

## Development

The repository provides configuration files for clangd and clang-tidy. They
expect you to be working on POSIX environments. For Windows, you will not get
fancy code completion and errors.

For clang-tidy, the script `tooling/clang-tidy.sh` can be run. If you want to
contribute something, please run clang-tidy to ensure that you didn't mess up
somewhere.
