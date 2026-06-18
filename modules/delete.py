import json, sys
if len(sys.argv) < 2:
    print('Usage: python3 delete.py <block_key> [field_name]')
    sys.exit(1)
target_key = sys.argv[1]
field_name = sys.argv[2] if len(sys.argv) > 2 else None
try:
    with open("north_shore_archive.json", "r+", encoding="utf-8") as f:
        data = json.load(f)
        if target_key in data:
            if field_name:
                if field_name in data[target_key]:
                    del data[target_key][field_name]
                    print(f'{{"status": "Deleted field \'{field_name}\' from \'{target_key}\'"}}')
                else:
                    print(f'{{"error": "Field \'{field_name}\' not found"}}')
            else:
                del data[target_key]
                print(f'{{"status": "Deleted entire block \'{target_key}\'"}}')
            f.seek(0)
            json.dump(data, f, indent=2)
            f.truncate()
        else:
            print(f'{{"error": "Block key \'{target_key}\' not found"}}')
except Exception as e:
    print(f'{{"error": "Deletion failed: {str(e)}"}}')
