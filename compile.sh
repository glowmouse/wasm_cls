#!/bin/bash

mkdir -p docs
emcc cls.cpp -O2 --shell-file $EMSDK/fastcomp/emscripten/src/shell_minimal.html -std=c++11 -s WASM=1  -o docs/index.html

