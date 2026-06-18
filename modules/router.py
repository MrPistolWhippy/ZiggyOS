import socket, json, sys, os

DB = "north_shore_archive.json"
PORT = 9999

def run_server():
    # Binds directly to the local hardware CPU loopback adapter
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        s.bind(("127.0.0.1", PORT))
        s.listen(1)
        while True:
            conn, addr = s.accept()
            req = conn.recv(1024).decode().strip().lower()
            if not req: continue
            
            # Internal Data Table Router Engine
            try:
                data = json.load(open(DB, "r"))
                if req == "all":
                    res = json.dumps(data, indent=2)
                elif req == "meta":
                    res = json.dumps(data.get("archive_metadata", {}), indent=2)
                elif req == "profile":
                    res = json.dumps(data.get("part_2_alumnus_profile", {}), indent=2)
                else:
                    # Direct inline string filtering
                    matches = {k: v for k, v in data.items() if req in json.dumps(v).lower()}
                    res = json.dumps({"query": req, "matches": matches}, indent=2)
            except Exception as e:
                res = f'{{"error": "Disk read failure: {str(e)}"}}'
                
            conn.sendall(res.encode())
            conn.close()
    except Exception as e:
        print(f"Router core halted: {e}")

def send_request(msg):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", PORT))
        s.sendall(msg.encode())
        print(s.recv(4096).decode())
        s.close()
    except:
        print('{"status": "offline_line_inactive", "reason": "Boot the background link first."}')

if __name__ == "__main__":
    if len(sys.argv) > 1:
        send_request(" ".join(sys.argv[1:]))
    else:
        print(f">> Local IPC Router Listening on 127.0.0.1:{PORT} [STRICT OFFLINE MODE]")
        run_server()
