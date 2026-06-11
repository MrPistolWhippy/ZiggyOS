#!/bin/sh
echo "============================================================"
echo "🕵️  ZIGGY-OS DEVELOPER WORKSPACE DIAGNOSTIC AUDIT"
echo "============================================================"

echo ""
echo "[1. DIRECTORY BLOAT & CLEANUP CANDIDATES]"
du -sh *.zip *.iso *.img *.bin 2>/dev/null | sort -rh

echo ""
echo "[2. UNTRACKED ENVIRONMENT FILES (GIT SENSITIVE)]"
git status -s | grep "??" | head -n 10

echo ""
echo "[3. BROKEN SYMBOLIC LINKS & PATH PATHOLOGY]"
find . -type l ! -exec test -e {} \; -print

echo ""
echo "[4. COMPILER DEPENDENCY STRUCTURAL DRIFT]"
if [ ! -f "src/io.h" ]; then
    echo "⚠️  CRITICAL ALERT: src/io.h is missing. Recompile will fail."
else
    echo "✅ Kernel header dependency 'src/io.h' is present."
fi

if [ ! -f "linker.ld" ]; then
    echo "⚠️  ALERT: linker.ld missing from project root folder."
else
    echo "✅ Linker script mapping profile 'linker.ld' is present."
fi

echo ""
echo "[5. LOG INTEGRITY & SEED CHECKPOINT VALIDATION]"
if [ -f "archive.db" ]; then
    python3 -c "
import sqlite3
try:
    conn = sqlite3.connect('archive.db')
    res = conn.execute('SELECT * FROM application_logs;').fetchall()
    print(f'✅ SQLite DB Operational. Active Tracking Sync Logs: {res}')
except Exception as e:
    print(f'❌ Database integrity failure or empty logs: {e}')
"
else
    echo "❌ ERROR: archive.db structural seed file missing from root path."
fi
echo "============================================================"
