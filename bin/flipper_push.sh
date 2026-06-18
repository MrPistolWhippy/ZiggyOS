#!/bin/bash
echo "Initiating Flipper repository push sequence..."
git push origin main
if [ $? -eq 0 ]; then
    echo "✔ Synchronization stream confirmed successful!"
    sleep 2
    clear
    echo "Console panel swept. Ready for next task."
else
    echo "❌ Error: Push sequence failed."
    exit 1
fi
