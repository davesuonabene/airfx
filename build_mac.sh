#!/bin/bash
set -e

echo "============================================================"
echo " Building AirFX Externals for macOS (Universal Binaries)"
echo "============================================================"

# Ensure we're in the project root
cd "$(dirname "$0")"

# 1. Configure the build using a distinct 'build_mac' folder
echo "[1/2] Configuring CMake..."
cmake -B build_mac

# 2. Build in Release mode. 
# --parallel 8 speeds up the build significantly by using multiple cores.
echo "[2/2] Building .mxo bundles..."
cmake --build build_mac --config Release --parallel 8

echo "============================================================"
echo " Build Complete!"
echo " Check the 'externals' folder for your .mxo files."
echo "============================================================"
