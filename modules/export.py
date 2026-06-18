import json
try:
    with open("north_shore_archive.json", "r", encoding="utf-8") as f:
        data = json.load(f)
    with open("archive_export.csv", "w", encoding="utf-8") as csv_file:
        csv_file.write("Block,Key,Value\n")
        for block, content in data.items():
            if isinstance(content, dict):
                for k, v in content.items():
                    val_str = str(v).replace('"', '""')
                    csv_file.write(f'"{block}","{k}","{val_str}"\n')
    print('{"status": "Export successful", "target": "archive_export.csv"}')
except Exception as e:
    print(f'{{"error": "Export failed: {str(e)}"}}')
