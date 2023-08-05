#!/bin/bash

project_dir="$PWD"
output_file=".clangd"

# Delete the existing output file if it exists
rm -f "$output_file"

cat <<EOT>> "$output_file"
CompileFlags:
  Add:
    - "-I$project_dir/"
    - "-I$project_dir/lib/thirdparty/libc++/include"
    - "-I$project_dir/lib/thirdparty/"
    - "-std=c++20"
    - "-Wpedantic"
    - "-DENABLE_TESTS"
    - "-Wextra"
    - "-Wall"

Diagnostics:
  ClangTidy:
    Add: 
      - modernize*
    Remove:
      - misc-unused-alias-decls
      - modernize-use-nodiscard
EOT
