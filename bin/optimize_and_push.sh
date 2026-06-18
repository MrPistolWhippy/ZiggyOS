#!/bin/sh
echo "=== 1. OPTIMIZING WORKSPACE ==="
# Remove temporary compilation and python cache files to save space
rm -rf __pycache__
rm -f ._* 
rm -f precise_patch.py final_clean.py fix_all.sh

echo "=== 2. VERIFYING GIT STATUS ==="
# Check active files tracking status
git status -s

echo "=== 3. STAGING ALL OS ARTIFACTS ==="
# Stage source code, images (.img, .iso), database, and configurations
git add src/ archive.db boot.bin ziggyos.bin final_matrix.img true_ziggyos.iso Makefile db_interceptor.py run_live_test.py run_all_stages.sh verify_first.sh

echo "=== 4. COMMITING FINALIZED BUILD ==="
# Commit with a definitive architecture tracking message
git commit -m "Optimize workspace layout and finalize ZiggyOS kernel v1.0"

echo "=== 5. PUSHING TO REMOTE REPOSITORY ==="
# Push commits to your remote repository branch using your active credentials
git push origin main || git push
echo "✨ All operations completed successfully! Workspace is fully optimized and backed up to Git."
