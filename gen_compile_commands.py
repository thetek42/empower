import json
import os

FILES: list[tuple[str, list[str]]] = [
    ("src/e_alloc.h", ["E_ALLOC_IMPL"]),
    ("src/e_arena.h", ["E_ARENA_IMPL"]),
    ("src/e_base64.h", ["E_BASE64_IMPL"]),
    ("src/e_bitvec.h", ["E_BITVEC_IMPL"]),
    ("src/e_cobs.h", ["E_COBS_IMPL"]),
    ("src/e_cobsr.h", ["E_COBSR_IMPL"]),
    ("src/e_cstr.h", ["E_CSTR_IMPL"]),
    ("src/e_debug.h", ["E_DEBUG_IMPL"]),
    ("src/e_fs.h", ["E_FS_IMPL", "E_RESULT_IMPL"]),
    ("src/e_ini.h", ["E_INI_IMPL", "E_PARSE_IMPL", "E_RESULT_IMPL"]),
    ("src/e_log.h", ["E_LOG_IMPL"]),
    ("src/e_macro.h", ["E_MACRO_IMPL"]),
    ("src/e_mem.h", ["E_MEM_IMPL"]),
    ("src/e_parse.h", ["E_PARSE_IMPL"]),
    ("src/e_rand.h", ["E_RAND_IMPL"]),
    ("src/e_rbuf.h", ["E_RBUF_IMPL", "E_RBUF_TYPE=int", "E_RBUF_NAME=E_Rbuf_Int", "E_RBUF_PREFIX=e_rbuf_int"]),
    ("src/e_result.h", ["E_RESULT_IMPL"]),
    ("src/e_str.h", ["E_STR_IMPL", "E_CSTR_IMPL"]),
    ("src/e_test.h", ["E_TEST_IMPL", "E_RESULT_IMPL"]),
    ("src/e_types.h", ["E_TYPES_IMPL"]),
    ("src/e_vec.h", ["E_VEC_IMPL", "E_VEC_TYPE=int", "E_VEC_NAME=E_Vec_Int", "E_VEC_PREFIX=e_vec_int"]),
    ("test/test.c", []),
    ("test/test_alloc.c", []),
    ("test/test_arena.c", []),
    ("test/test_base64.c", []),
    ("test/test_bitvec.c", []),
    ("test/test_cobs.c", []),
    ("test/test_cobsr.c", []),
    ("test/test_cstr.c", []),
    ("test/test_debug.c", []),
    ("test/test_fs.c", []),
    ("test/test_ini.c", []),
    ("test/test_log.c", []),
    ("test/test_macro.c", []),
    ("test/test_mem.c", []),
    ("test/test_parse.c", []),
    ("test/test_rand.c", []),
    ("test/test_rbuf.c", []),
    ("test/test_result.c", []),
    ("test/test_str.c", []),
    ("test/test_types.c", []),
    ("test/test_vec.c", []),
]

FLAGS = [
    "/usr/bin/clang",
    "-std=c23",
    "-Isrc",
    "-D_XOPEN_SOURCE=600",
    "-D_POSIX_C_SOURCE=200809L",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wdouble-promotion",
    "-Wconversion",
    "-pedantic",
    "-Wno-sign-conversion",
    "-Wno-unknown-attributes",
    "-DDEBUG",
    "-Og",
    "-ggdb3",
    "-fsanitize=undefined,address,leak",
    "-c",
]

cwd = os.getcwd()
output = []
for (filename, defines) in FILES:
    entry = dict()
    entry["arguments"] = list(FLAGS)
    for define in defines:
        entry["arguments"].append("-D" + define)
    entry["arguments"].append(filename)
    if filename.endswith(".c"):
        outputname = filename.rsplit("/", 1)[1].replace(".c", ".o")
        outputloc = "obj/posix-posix-clang-c23/" + outputname
        entry["arguments"].append("-o")
        entry["arguments"].append(outputloc)
        entry["output"] = cwd + "/" + outputloc
    entry["directory"] = cwd
    entry["file"] = cwd + "/" + filename
    output.append(entry)
json = json.dumps(output)
print(json)
