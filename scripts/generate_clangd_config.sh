#!/bin/bash

project_dir="$PWD"
output_file=".clangd"

# Delete the existing output file if it exists
rm -f "$output_file"

# Initialize empty flag array
compile_flags=(
  "-I$project_dir/"
  "-I$project_dir/lib/thirdparty/libc++/include"
  "-I$project_dir/lib/thirdparty/"
  "-std=c++20"
  "-Wpedantic"
  "-Wextra"
  "-Wall"
)

# Check for command-line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    -D*)
      compile_flags+=("$1")
      shift
      ;;
    *)
      echo "Unknown option: $1"
      exit 1
      ;;
  esac
done

cat <<EOT>> "$output_file"
Diagnostics:
  ClangTidy:
    Add: 
      - modernize*
    Remove:
      - misc-unused-alias-decls
      - modernize-use-nodiscard
      - modernize-avoid-c-arrays

CompileFlags:
  Add:
EOT

for flag in "${compile_flags[@]}"; do
  echo "      - \"$flag\"" >> "$output_file"
done
