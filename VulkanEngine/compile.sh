#!/bin/sh
# compile.sh - Linux/macOS: glslc GLSL -> SPIR-V
set -e
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SHADER_DIR="$SCRIPT_DIR/shaders"
GLSLC="${VULKAN_SDK:-}/bin/glslc"
if [ ! -x "$GLSLC" ]; then GLSLC="$(command -v glslc || true)"; fi
if [ -z "$GLSLC" ] || [ ! -x "$GLSLC" ]; then GLSLC="$(command -v glslangValidator || true)"; USE_GLSLANG=1; fi
if [ -z "$GLSLC" ]; then echo "ERROR: glslc/glslangValidator not found. Install Vulkan SDK."; exit 1; fi
echo "Using: $GLSLC"
cd "$SHADER_DIR"
if [ "${USE_GLSLANG:-0}" = "1" ]; then
  "$GLSLC" -V triangle.vert -o triangle.vert.spv
  "$GLSLC" -V triangle.frag -o triangle.frag.spv
else
  "$GLSLC" triangle.vert -o triangle.vert.spv
  "$GLSLC" triangle.frag -o triangle.frag.spv
fi
echo "OK: $(ls -l *.spv)"
