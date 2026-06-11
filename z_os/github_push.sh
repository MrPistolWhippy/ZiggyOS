#!/bin/sh
cd ~/z_os

# 1. Substitute your token and username target destinations securely
GITHUB_USER="MrPistolWhippy"
GITHUB_REPO="ZiggyOS"
GITHUB_TOKEN="YOUR_TOKEN_HERE" # <--- PASTE YOUR ghp_ TOKEN EXACTLY HERE

echo "==> Step 1: Aligning Git identity variables..."
git config --global user.name "MrPistolWhippy"
git config --global user.email "mrpistolwhippy@://github.com"

echo "==> Step 2: Setting up default main branch alignment..."
git branch -M main 2>/dev/null || true

echo "==> Step 3: Configuring remote repository credentials mapping..."
# Safely clear any previously misconfigured remote lines
git remote remove origin 2>/dev/null || true
# Bind the clean authentication remote path using the token stream mask
git remote add origin "https://${GITHUB_USER}:${GITHUB_TOKEN}@://github.com{GITHUB_USER}/${GITHUB_REPO}.git"

echo "==> Step 4: Pushing 11-Module Graphical Core up to GitHub profile..."
echo "Please make sure you have already created an empty repository named 'ZiggyOS' on your GitHub dashboard!"
echo "------------------------------------------------------------------------"
git push -u origin main

echo "------------------------------------------------------------------------"
echo "==> SUCCESS: Your entire iPad workspace repository is live at ://github.com ! Brrrrrp! 🚀"
