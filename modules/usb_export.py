import shutil, os
SOURCE_FILE = "north_shore_archive.json"
TARGET_MOUNT_DIR = "/mnt/usb" 
TARGET_FILE = os.path.join(TARGET_MOUNT_DIR, "north_shore_archive_external.json")

try:
    if not os.path.exists(TARGET_MOUNT_DIR):
        os.makedirs(TARGET_MOUNT_DIR, exist_ok=True)
    shutil.copy2(SOURCE_FILE, TARGET_FILE)
    print(f'{{"status": "Hardware Mirror Complete", "bytes_committed": {os.path.getsize(TARGET_FILE)}, "mount_path": "{TARGET_FILE}"}}')
except Exception as e:
    print(f'{{"status": "Export Error", "reason": "Ensure USB-C drive is mounted or directory is accessible: {str(e)}"}}')
