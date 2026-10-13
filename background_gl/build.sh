#!/bin/bash

# Build script for background_gl
mkdir -p dist

echo "Building background_gl..."

em++ src/main.cpp \
  -O3 \
  -s USE_GLFW=3 \
  -s FULL_ES2=1 \
  -s WASM=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -o dist/bg_gl.js

if [ $? -eq 0 ]; then
  echo "Build successful! Output generated in background_engine/dist/"
else
  echo "Build failed. Check Emscripten installation and errors above."
fi
