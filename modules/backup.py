import shutil
try:
    shutil.copy2("north_shore_archive.json", "north_shore_archive_bak.json")
    print('{"status": "Backup successful", "target": "north_shore_archive_bak.json"}')
except Exception as e:
    print(f'{{"error": "Backup failed: {str(e)}"}}')
