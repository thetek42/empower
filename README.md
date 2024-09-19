# empower

Empower is a C library consisting of a collection of modules that empower you to write applications
in C faster and with more joy. Empower is compiler-agnostic, supports all major C standards (C89,
C99, C11 and C23) and is fully configurable.

## Modules

The following table shows the available modules alongside how well they are supported in different C
standards. :white_check_mark: = full support, :construction: = partial support, :x: = no support

| Module         | Description                    | C89                | C99                | C11                | C23                |
| -------------- | ------------------------------ | ------------------ | ------------------ | ------------------ | ------------------ |
| **e_alloc**    | Memory allocation              | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_compat**   | C standard compatibility       | :construction:     | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_compiler** | Compiler attributes and compat | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_cstr**     | String utility functions       | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_debug**    | Debugging utilities            | :x:                | :x:                | :white_check_mark: | :white_check_mark: |
| **e_log**      | Logging                        | :construction:     | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_macro**    | Macro helpers                  | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_result**   | Result type                    | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_test**     | Testing                        | :construction:     | :construction:     | :construction:     | :white_check_mark: |
| **e_types**    | Common type aliases            | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **e_vec**      | Vectors / dynamic arrays       | :white_check_mark: | :white_check_mark: | :white_check_mark: | :white_check_mark: |
