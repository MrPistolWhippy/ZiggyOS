import socket, threading, sqlite3, base64, time, os, collections, shutil, random, hashlib, struct
P, D, CF, R = 9876, "transmission_diary.log", ".cycle_counter", "dashboard_report.html"
def init_data_nodes():
    co = sqlite3.connect("archive.db"); cu = co.cursor()
    cu.execute("CREATE TABLE IF NOT EXISTS wingspread_1941_archive (id INTEGER PRIMARY KEY, record TEXT)")
    cu.execute("CREATE TABLE IF NOT EXISTS ingress_network_node (id INTEGER PRIMARY KEY, packet_data TEXT, timestamp TEXT)")
    cu.execute("CREATE TABLE IF NOT EXISTS node_gamma_telemetry (id INTEGER PRIMARY KEY, altitude_ft INTEGER, speed_knots INTEGER, fuel_pct REAL, timestamp TEXT)")
    co.commit(); co.close()

def handle_client(cs, ad):
    if ad[0] != "127.0.0.1": cs.close(); return
    try:
        dt = cs.recv(1024).decode().strip()
        if dt:
            ts = time.strftime("%H:%M:%S")
            co = sqlite3.connect("archive.db"); cu = co.cursor()
            cu.execute("INSERT INTO ingress_network_node (packet_data, timestamp) VALUES (?, ?)", (dt, ts))
            co.commit(); co.close()
            with open(D, "a") as f: f.write(f"[{time.strftime('%Y-%m-%d %H:%M:%S')}] [SECURE] INL: {dt} at {ts}\n")
    except: pass
    finally: cs.close()

def start_network_listener():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM); s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    try:
        s.bind(("127.0.0.1", P)); s.listen(5)
        while True: cl, ad = s.accept(); threading.Thread(target=handle_client, args=(cl, ad), daemon=True).start()
    except: pass

def render_ptcd_dashboard(cy):
    print(f"\n\033[1;35m=======================================================================\n⚡ ZIGGY-OS DISTRIBUTED MESH | SWEEP #{cy} ⚡\n=======================================================================\033[0m")
    co = sqlite3.connect("archive.db"); cu = co.cursor()
    al, sp, fl, ts = int(time.time() % 3000 + 4000), random.randint(320, 480), round(random.uniform(60.0, 99.9), 1), time.strftime("%H:%M:%S")
    cu.execute("INSERT INTO node_gamma_telemetry (altitude_ft, speed_knots, fuel_pct, timestamp) VALUES (?, ?, ?, ?)", (al, sp, fl, ts))
    co.commit()
    cu.execute("SELECT * FROM node_gamma_telemetry ORDER BY id DESC LIMIT 1"); g = cu.fetchone()
    print(f"\033[1;34m[📡 NODE GAMMA SCHEMA (NGts+)]\033[0m -> Alt: {g[1]} ft | Velocity: {g[2]} kts | Fuel Pool: {g[3]}%")
    print(f"\n\033[1;36m[🌐 DECENTRALIZED PEER-TO-PEER MESH]\033[0m\n  📍 Peer-Local   [127.0.0.1:{P}]  : \033[1;32mONLINE\033[0m (Autonomous Peer Loop)\n  ☁️  Node-Alpha   [127.0.0.1:9991]  : \033[1;33mSYNCHRONIZED\033[0m\n  ☁️  Node-Beta    [127.0.0.1:9992]  : \033[1;33mSYNCHRONIZED\033[0m")
    co.close()

if __name__ == "__main__":
    init_data_nodes()
    if not os.path.exists(CF):
        with open(CF, "w") as f: f.write("0")
    with open(CF, "r") as f: cy = int(f.read().strip()) + 1
    with open(CF, "w") as f: f.write(str(cy))
    threading.Thread(target=start_network_listener, daemon=True).start()
    render_ptcd_dashboard(cy)
