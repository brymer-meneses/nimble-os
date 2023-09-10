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

def writeln(f, level, string):
    indents = '  ' * level
    f.write(f"{indents}{string}\n")

if __name__ == "__main__":
    with open(".clangd", "w") as f:
        writeln(f, 0, "Diagnostics:")
        writeln(f, 1, "ClangTidy:")

        writeln(f, 2, "Add:")
        for flag in clangtidy_flags["add"]:
            writeln(f, 3, f"- {flag}")

        writeln(f, 2, "Remove:")
        for flag in clangtidy_flags["remove"]:
            writeln(f, 3, f"- {flag}")

        writeln(f, 0, "CompileFlags:")
        writeln(f, 1, "Add:")

        for flag in compile_flags:
            writeln(f, 2, f'- "-{flag}"')
        for dir in include_dirs:
            writeln(f, 2, f'- "-I{os.path.join(os.getcwd(),dir)}"')
