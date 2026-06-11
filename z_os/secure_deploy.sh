#!/bin/sh
echo "Enter your FRESH GitHub Token:"
read FRESH_TOKEN

# Construct the remote destination natively in memory
git remote remove origin 2>/dev/null
git remote add origin "https://MrPistolWhippy:${FRESH_TOKEN}@://github.com"

echo "==> Staging workspace updates..."
git add .
git commit -m "Pipeline update: Deploy GitHub Actions toolchain" 2>/dev/null || true

echo "==> Uploading kernel repository to GitHub compiler..."
git push -u origin main || git push -u origin master
