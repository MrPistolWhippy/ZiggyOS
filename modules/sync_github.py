import urllib.request
import json
import base64

token = "ghp_F88UpTW4gzS8V62k0zbhHDHZio4qed4JTFYP"
repo = "MrPistolWhippy/ziggy-kernel-os"
files_to_sync = ["Makefile", "src/archive.c", "src/archive.h", "src/main.c"]

headers = {
    "Authorization": f"Bearer {token}",
    "Accept": "application/vnd.github.v3+json",
    "User-Agent": "iSH-iPadOS-KernelEngine"
}

for filepath in files_to_sync:
    try:
        # Corrected strict REST API endpoint target location configuration
        url = f"https://github.com{repo}/contents/{filepath}"
        
        # Step 1: Query GitHub to locate the unique structural tracking blob SHA
        req = urllib.request.Request(url, headers=headers)
        sha = None
        try:
            with urllib.request.urlopen(req) as response:
                sha = json.loads(response.read().decode())["sha"]
        except Exception:
            pass # File does not exist upstream yet

        # Step 2: Extract local binary bytes and encode to a flat text stream base64 signature
        with open(filepath, "rb") as f:
            content_bytes = f.read()
        
        payload = {
            "message": f"sync: flash {filepath} payload via direct REST runtime",
            "content": base64.b64encode(content_bytes).decode("utf-8")
        }
        if sha:
            payload["sha"] = sha

        data = json.dumps(payload).encode("utf-8")
        req_put = urllib.request.Request(url, data=data, headers=headers, method="PUT")
        
        with urllib.request.urlopen(req_put) as response:
            if response.status in [200, 201]:
                print(f"[✔] Successfully synced asset: {filepath}")
    except Exception as e:
        print(f"[!] Failed to process sync execution loop for {filepath}: {e}")
