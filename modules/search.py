import json, sys
q = sys.argv[1] if len(sys.argv) > 1 else "shriane"
try:
    with open("north_shore_archive.json", "r", encoding="utf-8") as f:
        data = json.load(f)
    results = {k: v for k, v in data.items() if q.lower() in json.dumps(v).lower()}
    print(json.dumps({"query": q, "matches": results}, indent=2))
except Exception as e:
    print(f"Direct file query failed: {e}")
