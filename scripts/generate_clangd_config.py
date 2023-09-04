#!/usr/bin/env python3

import os

clangtidy_flags = {
    "add": [
        "modernize*",
    ],
    "remove": [
        "misc-unused-alias-decls",
        "modernize-use-nodiscard",
        "modernize-avoid-c-arrays",
    ]
}

compile_flags = [
    "std=c++20",
    "Wpedantic",
    "Wextra",
    "Wall",
]

include_dirs= [
    "lib/thirdparty/libc++/include",
    "lib/thirdparty/",
    "",
]

def writeline(f, level, string):
    indents = '  ' * level
    f.write(f"{indents}{string}\n")

with open(".clangd", "w") as f:
    writeline(f, 0, "Diagnostics:")
    writeline(f, 1, "ClangTidy:")

    writeline(f, 2, "Add:")
    for flag in clangtidy_flags["add"]:
        writeline(f, 3, f"- {flag}")

    writeline(f, 2, "Remove:")
    for flag in clangtidy_flags["remove"]:
        writeline(f, 3, f"- {flag}")

    writeline(f, 0, "CompileFlags:")
    writeline(f, 1, "Add:")
    for flag in compile_flags:
        writeline(f, 2, f"- \"-{flag}\"")
    for dir in include_dirs:
        writeline(f, 2, f"- \"-I{os.path.join(os.getcwd(),dir)}\"")
