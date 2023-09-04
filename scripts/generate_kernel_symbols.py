#!/usr/bin/env python3

import subprocess
import os
from typing import Optional

def run_command(command: str) -> Optional[str]:
    result = subprocess.run(command, shell=True, 
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, 
                            text=True)
    if result.returncode == 0:
        return result.stdout
    return None

header = """\
#pragma once

#include <cstdint>
#include <cstddef>

struct Symbol {
    uint64_t address;
    size_t size;
    const char* name;
};

__attribute__((section(".symbols")))
Symbol symbols[] 
= {
"""

def create_output_file(symbols: str) -> None:

    with open("build/temp.h", 'w') as file:
        file.write(header)

        for line in symbols.splitlines():
            columns = line.split(" ")
            if len(columns) < 4:
                continue

            address = columns[0]
            size = columns[1]
            name = columns[3]

            file.write(f"\t{{ 0x{address}, 0x{size}, \"{name}\" }},\n")

        file.write("};")

    
    subprocess.run("c++filt -i < build/temp.h > build/symbols.h", shell=True)
    os.remove("build/temp.h")

def require_program(program: str) -> None:
    try:
        subprocess.check_call(['which', program], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except subprocess.CalledProcessError:
        raise FileNotFoundError(f"The program {program} is not available--make sure it exists on your path.")
        

if __name__ == "__main__":
    require_program("nm")
    require_program("c++filt")

    print("Generating kernel symbols ...")
    symbols = run_command("nm build/debug/kernel.elf -S")

    if symbols is not None:
        create_output_file(symbols)


