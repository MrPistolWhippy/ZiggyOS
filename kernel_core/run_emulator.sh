#!/bin/sh
# ZiggyOS Automated Artifact Downloader & Boot Script

REPO="MrPistolWhippy/ZiggyOS"
ARTIFACT_NAME="ziggyos-boot-image"
ZIP_FILE="ziggyos_artifact.zip"
ISO_FILE="ziggyos.iso"

echo "=== 1. Checking GitHub Authentication ==="
if ! command -v gh &> /dev/null; then
    echo "Error: GitHub CLI (gh) is not installed. Run: apk add github-cli"
    exit 1
fi

echo "=== 2. Fetching Latest Successful Artifact ==="
# Download the latest v4 zip artifact directly via the GitHub CLI
gh run download --repo "$REPO" --name "$ARTIFACT_NAME" --dir . 2>/dev/null

# Fallback rename check if downloaded as a generic zip name
if [ -f "$ARTIFACT_NAME.zip" ]; then
    mv "$ARTIFACT_NAME.zip" "$ZIP_FILE"
elif [ -f "ziggyos.iso" ]; then
    echo "ISO already present locally."
else
    # Query GitHub API directly to find and curl the zip file asset
    echo "Falling back to direct API curl download..."
    RUN_ID=$(curl -s "https://github.com" | grep -m 1 '"id":' | awk '{print $2}' | tr -d ',')
    ART_URL=$(curl -s "https://github.com" | grep "archive_download_url" | head -n 1 | awk -F '"' '{print $4}')
    
    if [ -n "$ART_URL" ]; then
        curl -L -H "Authorization: token $(gh auth token)" -o "$ZIP_FILE" "$ART_URL"
    fi
fi

echo "=== 3. Extracting Bootable ISO Image ==="
if [ -f "$ZIP_FILE" ]; then
    unzip -o "$ZIP_FILE"
    rm -f "$ZIP_FILE"
fi

if [ ! -f "$ISO_FILE" ]; then
    echo "Error: Failed to fetch or extract $ISO_FILE!"
    exit 1
fi

echo "=== 4. Booting ZiggyOS inside QEMU x86 Hardware Emulator ==="
echo "Directing video frame buffer into terminal graphics matrix..."
echo "Press Ctrl+A then X to exit the emulator safely."
echo "--------------------------------------------------------"

# Launches the virtual hardware matrix using text-only redirection
qemu-system-x86_64 -nographic -cdrom "$ISO_FILE" -m 32
