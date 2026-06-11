#!/bin/sh
set -e

echo "⚙️ Running system build via unified Makefile layout..."
make clean && make

echo "📦 Executing Python Deployment Packaging Asset Engine..."
python3 pack_firmware.py

echo "🔍 Analyzing math engine boundaries with Radare2..."
r2 -q -c "s sym.fixed_sin; pd 5" ziggyos.elf
