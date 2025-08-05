#!/bin/bash

set -e  # hata varsa çık

mkdir -p ./bin
mkdir -p ./build

std=-std=c++17

arg="$1"

if [[ "$arg" == "tok" || -z $arg ]]; then
        srcs=(programs/tokenizer.cxx)
        while IFS= read -r file; do
                srcs+=("$file")
        done < <(find src/tokenizer/ -type f -regex '.*\.\(cxx\|cpp\)$')

        echo "Tokenizer source files:"
        printf '  %s\n' "${srcs[@]}"

        objs=()
        for src in "${srcs[@]}"; do
                obj="./build/$(echo "$src" | tr "/" "_").o"
                echo "🔧 Compiling $src -> $obj"
                g++ $std -march=native -c -I./include "$src" -o "$obj"
                objs+=("$obj")
        done

        echo "Object files:"
        printf '  %s\n' "${objs[@]}"

        echo "🔗 Linking -> bin/tokenizer"
        g++ $std -I./include "${objs[@]}" -o bin/tokenizer
        echo "✅ Build success: ./bin/tokenizer"
fi

if [[ "$arg" == "par" || -z $arg ]]; then
        srcs=(programs/tokenparser.cxx)
        while IFS= read -r file; do
                srcs+=("$file")
        done < <(find src/tokenparser/ src/tokenizer/tokens.cxx -type f -regex '.*\.\(cxx\|cpp\)$')

        echo "Tokenparser source files:"
        printf '  %s\n' "${srcs[@]}"

        objs=()
        for src in "${srcs[@]}"; do
                obj="./build/$(echo "$src" | tr "/" "_").o"
                echo "🔧 Compiling $src -> $obj"
                g++ $std -march=native -c -I./include "$src" -o "$obj"
                objs+=("$obj")
        done

        echo "Object files:"
        printf '  %s\n' "${objs[@]}"

        echo "🔗 Linking -> bin/tokenparser"
        g++ $std -I./include "${objs[@]}" -o bin/tokenparser
        echo "✅ Build success: ./bin/tokenparser"
fi
