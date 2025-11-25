# Empower Support

## Legend

| Symbol | Meaning |
| ------ | ------- |
| ✅ | Fully supported |
| 🔶 | Not all features are available |
| ❌ | Not supported |

## Compilers

| Compiler  | Supported |
| --------- | --------- |
| GCC       | ✅ |
| Clang     | ✅ |
| TinyCC    | ✅ |
| MinGW GCC | ✅ |
| MSVC      | ✅ |

Others likely work as well, but are not actively tested.

The following versions are used for testing:
- GCC: 15.2.1
- Clang: 21.1.4
- TinyCC: 0.9.28rc
- MinGW GCC: 15.2.0 (x86_64-w64-mingw32-gcc)
- MSVC: 19.44.35221 (cl.exe)

## C Standard Versions

| Module   | C89 | C99 | C11 | C23 |
| -------- | --- | --- | --- | --- |
| e_arena  | ✅ | ✅ | ✅ | ✅ |
| e_base64 | ✅ | ✅ | ✅ | ✅ |
| e_bitvec | ✅ | ✅ | ✅ | ✅ |
| e_cobs   | ✅ | ✅ | ✅ | ✅ |
| e_cobsr  | ✅ | ✅ | ✅ | ✅ |
| e_cstr   | ✅ | ✅ | ✅ | ✅ |
| e_debug  | 🔶 | 🔶 | ✅ | ✅ |
| e_ini    | ✅ | ✅ | ✅ | ✅ |
| e_log    | ❌ | ✅ | ✅ | ✅ |
| e_macro  | 🔶 | 🔶 | ✅ | ✅ |
| e_mem    | 🔶 | ✅ | ✅ | ✅ |
| e_rand   | ❌ | 🔶 | ✅ | ✅ |
| e_stdc   | ✅ | ✅ | ✅ | ✅ |
| e_test   | ✅ | ✅ | ✅ | ✅ |

## Platforms

| Module   | POSIX | Windows | Freestanding |
| -------- | --- | --- | --- |
| e_arena  | ✅ | ✅ | ✅ |
| e_base64 | ✅ | ✅ | ✅ |
| e_bitvec | ✅ | ✅ | ✅ |
| e_cobs   | ✅ | ✅ | ✅ |
| e_cobsr  | ✅ | ✅ | ✅ |
| e_cstr   | ✅ | ✅ | ❌ |
| e_debug  | ✅ | ✅ | ❌ |
| e_ini    | ✅ | ✅ | ✅ |
| e_log    | ✅ | ✅ | ❌ |
| e_macro  | ✅ | ✅ | ✅ |
| e_mem    | ✅ | ✅ | 🔶 |
| e_rand   | ✅ | ✅ | ✅ |
| e_stdc   | ✅ | ✅ | ✅ |
| e_test   | ✅ | ✅ | ❌ |

Note on the used platform names:
- POSIX = Linux, macOS, BSD and similar
- Freestanding = System without standard library
  - Note that the headers stdbool.h, stddef.h and stdint.h will still be used
    by Empower, since these are usually still available.

## CPU Architectures

| Architecture | Supported |
| ------------ | --------- |
| x86-64       | ✅ |

Others likely work as well, but are not actively tested.
