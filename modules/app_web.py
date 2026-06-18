#!/usr/bin/env python3
import http.server, socketserver, os
class HUDHandler(http.server.SimpleHTTPRequestHandler):
    def translate_path(self, path): return "/root/dashboard.html" if path == "/" else super().translate_path(path)
    def log_message(self, *a): return
socketserver.TCPServer.allow_reuse_address = True
try:
    with socketserver.TCPServer(("0.0.0.0", 8080), HUDHandler) as h:
        print("\033[1;32m[+] ZIGGYOS WEB HUD LIVE AT http://127.0.0.1:8080\033[0m"); h.serve_forever()
except Exception as e: print(e)
