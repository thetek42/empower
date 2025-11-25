STDC_C89 = 0
STDC_C99 = 1
STDC_C11 = 2
STDC_C23 = 3

STDC_VERSIONS = {
    STDC_C89: "c89",
    STDC_C99: "c99",
    STDC_C11: "c11",
    STDC_C23: "c23",
}

TARGETS = {
    "native-gcc": {
        "compiler": "gcc",
        "flags": [
            "-Og",
            "-ggdb",
            "-fsanitize=undefined,address,leak",
            "-fanalyzer",
            "-I../empower",
            "-Wall",
            "-Wextra",
            "-Wpedantic",
            "-Wdouble-promotion",
            "-Wconversion",
            "-Werror",
            "-Wno-error=unused",
            "-Wno-error=unused-but-set-variable",
            "-Wno-error=unused-parameter",
            "-Wno-error=unused-variable",
        ],
        "stdcflags": ["-std=c89", "-std=c99", "-std=c11", "-std=c23"],
        "outflag": ["-o", "$FILE$"],
        "outsuffix": "",
        "linkflags": [],
    },

    "native-clang": {
        "compiler": "clang",
        "flags": [
            "-Og",
            "-ggdb",
            "-fsanitize=undefined,address,leak",
            "-I../empower",
            "-Wall",
            "-Wextra",
            "-Wpedantic",
            "-Wdouble-promotion",
            "-Wconversion",
            "-Werror",
            "-Wno-error=unused",
            "-Wno-error=unused-but-set-variable",
            "-Wno-error=unused-parameter",
            "-Wno-error=unused-variable",
        ],
        "stdcflags": ["-std=c89", "-std=c99", "-std=c11", "-std=c23"],
        "outflag": ["-o", "$FILE$"],
        "outsuffix": "",
        "linkflags": [],
    },

    "native-tcc": {
        "compiler": "tcc",
        "flags": [
            "-gdwarf",
            "-b",
            "-I../empower",
            "-Wall",
            "-Werror",
        ],
        "stdcflags": [None, "-std=c99", "-std=c11", None],
        "outflag": ["-o", "$FILE$"],
        "outsuffix": "",
        "linkflags": [],
    },

    "mingw-x64": {
        "compiler": "x86_64-w64-mingw32-gcc",
        "flags": [
            "-Og",
            "-ggdb",
            "-fanalyzer",
            "-I../empower",
            "-Wall",
            "-Wextra",
            "-Wpedantic",
            "-Wdouble-promotion",
            "-Wconversion",
            "-Werror",
            "-Wno-error=unused",
            "-Wno-error=unused-but-set-variable",
            "-Wno-error=unused-parameter",
            "-Wno-error=unused-variable",
            "--static",
        ],
        "stdcflags": ["-std=c89", "-std=c99", "-std=c11", "-std=c23"],
        "outflag": ["-o", "$FILE$"],
        "outsuffix": ".exe",
        "linkflags": [],
    },

    "msvc": {
        "compiler": "cl",
        "flags": [
            "/nologo",
            "/I../empower",
            "/D_CRT_SECURE_NO_WARNINGS",
            "/W4",
            "/WX",
            "/permissive-",
            "/Od",
            "/Zi",
        ],
        "stdcflags": ["/Za", "", "/std:c11", "/std:clatest"],
        "outflag": ["/Fe:$FILE$"],
        "outsuffix": ".exe",
        "linkflags": [],
    },
}
