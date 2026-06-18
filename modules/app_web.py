import json, sys
from http.server import BaseHTTPRequestHandler, HTTPServer

DB = "north_shore_archive.json"

HTML_TEMPLATE = """<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Offline Data Hub</title>
    <style>
        body { font-family: -apple-system, sans-serif; background: #121212; color: #e0e0e0; padding: 20px; }
        .card { background: #1e1e1e; padding: 15px; border-radius: 8px; margin-bottom: 15px; border-left: 4px solid #008080; }
        h2 { color: #00ffff; margin-top: 0; }
        pre { background: #2d2d2d; padding: 10px; border-radius: 4px; overflow-x: auto; font-size: 13px; color: #fff; }
        .btn { background: #008080; color: white; border: none; padding: 10px 15px; border-radius: 4px; cursor: pointer; }
    </style>
</head>
<body>
    <h1>[MODE: NADA FLUX] Local Data Core</h1>
    <p>Status: Connected to Internal Loopback Bus (Offline)</p>
    
    <div class="card">
        <h2>Scott Brodie (Rangitoto Profile)</h2>
        <pre>__PROFILE_DATA__</pre>
    </div>
    
    <div class="card">
        <h2>Andrew Morrison (Irrom Profile)</h2>
        <pre>__IRROM_DATA__</pre>
    </div>
    
    <button class="btn" onclick="location.reload()">Refresh Memory Rails</button>
</body>
</html>"""

class WebHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == "/":
            self.send_response(200)
            self.send_header("Content-Type", "text/html")
            self.end_headers()
            try:
                data = json.load(open(DB, "r"))
                p1 = json.dumps(data.get("part_2_alumnus_profile", {}), indent=2)
                p2 = json.dumps(data.get("part_3_industry_cornerstone", {}), indent=2)
                output = HTML_TEMPLATE.replace("__PROFILE_DATA__", p1).replace("__IRROM_DATA__", p2)
            except Exception as e:
                output = f"<h1>Disk Read Error: {e}</h1>"
            self.wfile.write(output.encode())
        else:
            self.send_error(404)

if __name__ == "__main__":
    print("Web UI running locally on http://127.0.0.1:8080")
    HTTPServer(("127.0.0.1", 8080), WebHandler).serve_forever()
