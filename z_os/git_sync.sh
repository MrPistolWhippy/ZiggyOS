#!/bin/sh
cd ~/z_os

echo "==> Step 1: Synchronizing Git Workspace Repository..."
# Force tracking of all source configuration and scripting file targets recursively
git add *.c *.h *.asm *.sh Makefile 2>/dev/null || true
git add src/*.c src/*.h src/*.asm 2>/dev/null || true

# Explicitly tell Git to ignore heavy, temporary intermediate object files
git rm --cached *.o *.bin *.iso iso_root/* 2>/dev/null || true

# Commit the tracking adjustments natively into your history tree block
git commit -m "Milestone: Complete 11-Module Compositor Matrix with Minimize & Theme Cycle Engines" 2>/dev/null || echo "[+] Repository mapping fully up to date."

echo "==> Step 2: Triggering Unified 11-Module Subsystem Build Pass..."
./build.sh

echo "==> Step 3: Exporting Production Kernel Binary to iPad Files App..."
cp ziggyos.bin /root/'Ish files'/
echo "==> SUCCESS: Repository tracking saved and ziggyos.bin fully deployed! Brrrrrp! 🚀"
