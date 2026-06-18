import os
import subprocess
import time

print("Initiating Flipper repository push sequence...")

# Run the git push command
result = subprocess.run(["git", "push", "origin", "main"])

# Check the exit status code
if result.returncode == 0:
    print("------------------------------------------------")
    print("✔ Synchronization stream confirmed successful!")
    print("------------------------------------------------")
    time.sleep(2)
    os.system("clear")
    print("Console panel swept. Ready for next task.")
else:
    print("------------------------------------------------")
    print("❌ Error: Push sequence failed. Console not cleared.")
