# Empower

Empower is a collection of header-only libraries for C that empower you to write
applications faster and with more joy.

## Modules

Empower contains the following modules:

| Category        | Module                               | Description                         |
| --------------- | ------------------------------------ | ----------------------------------- |
| Strings         | [**e_cstr**](./empower/e_cstr.h)     | String utility functions            |
|                 | [**e_sb**](./empower/e_sb.h)         | String builder                      |
|                 | [**e_sv**](./empower/e_sv.h)         | String view                         |
| Data structures | [**e_da**](./empower/e_da.h)         | Generic dynamic arrays              |
|                 | [**e_rbuf**](./empower/e_rbuf.h)     | Generic ringbuffer                  |
|                 | [**e_bitvec**](./empower/e_bitvec.h) | Bit array                           |
| Algorithms      | [**e_base64**](./empower/e_base64.h) | Base64 encoding/decoding            |
|                 | [**e_bcd**](./empower/e_bcd.h)       | Binary-coded decimals               |
|                 | [**e_cobs**](./empower/e_cobs.h)     | COBS encoding/decoding              |
|                 | [**e_cobsr**](./empower/e_cobsr.h)   | COBS/R encoding/decoding            |
| File formats    | [**e_ini**](./empower/e_ini.h)       | INI file parsing                    |
| Allocation      | [**e_alloc**](./empower/e_alloc.h)   | Memory allocation                   |
|                 | [**e_arena**](./empower/e_arena.h)   | Arena allocator                     |
| Utilities       | [**e_debug**](./empower/e_debug.h)   | Debugging utilities                 |
|                 | [**e_log**](./empower/e_log.h)       | Logging                             |
|                 | [**e_macro**](./empower/e_macro.h)   | Macro helpers                       |
|                 | [**e_mem**](./empower/e_mem.h)       | Memory manipulation                 |
|                 | [**e_rand**](./empower/e_rand.h)     | Randomization                       |
| Compatibility   | [**e_compat**](./empower/e_compat.h) | C standard / compiler compatibility |
|                 | [**e_stdc**](./empower/e_stdc.h)     | Uniform feature test macros         |
| Testing         | [**e_test**](./empower/e_test.h)     | Testing                             |

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
