import urllib.request, json
url = "https://fis-ski.com"
print(">> Initializing native HTTP GET connection to public registry...")
try:
    req = urllib.request.Request(
        url, 
        headers={'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)'}
    )
    with urllib.request.urlopen(req, timeout=10) as response:
        html_payload = response.read().decode('utf-8', errors='ignore')
        print(f'{{"status": "Connection verified", "bytes_received": {len(html_payload)}}}')
        if "Brodie" in html_payload or "Hubbard" in html_payload:
            print(">> Live Update Alert: Target identification indicators located in public stream.")
except Exception as e:
    print(f'{{"status": "Network block", "error": "External DNS or route unavailable: {str(e)}"}}')
