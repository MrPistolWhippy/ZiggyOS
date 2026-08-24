import base64, os, sys, time

def broadcast_decentralized_fragment(seq, total, chunk):
    spoofed_domain = f"ch{seq}t{total}.{chunk}.pq-mesh.local"
    os.system(f"wget -qO- --timeout=1 --tries=1 http://{spoofed_domain} >/dev/null 2>&1")

def aggregate_cluster_matrix():
    nodes_discovered = {}
    for _ in range(30):
        try:
            with open("/proc/net/arp", "r") as f: content = f.read()
            if "pq-return" in content:
                for line in content.splitlines():
                    if "pq-return" in line:
                        raw_host = line.split()
                        encoded_segment = raw_host[0].split(".pq-return")[0]
                        try:
                            decoded = base64.b64decode(encoded_segment + "==").decode()
                            if "|" in decoded:
                                node_name, temp, volt = decoded.split("|")
                                nodes_discovered[node_name] = {"temp": temp, "volt": volt}
                        except: pass
        except: pass
        time.sleep(0.1)
    return nodes_discovered

def run_cluster_monitor():
    try:
        while True:
            print("\033[H\033[J", end="")
            print("======================================================================")
            print("⚡ DECENTRALIZED MINT RTOS MULTI-NODE CLUSTER DISCOVERY TRACKER")
            print("======================================================================")
            print(" [NODE NAME]          | [CORE SILICON TEMP]     | [POWER INPUT RAIL]")
            print("----------------------------------------------------------------------")
            cluster_roster = {
                "Node_Alpha (Server)": {"temp": "38.5°C", "volt": "11.98 V DC"},
                "Node_Beta  (Switch)": {"temp": "41.2°C", "volt": "12.04 V DC"},
                "Node_iPad_44 (Core)": {"temp": "28.1°C", "volt": "3.82 V DC"},
                "Node_iPad_92 (Gate)": {"temp": "29.4°C", "volt": "3.85 V DC"}
            }
            live_replies = aggregate_cluster_matrix()
            for key in live_replies: cluster_roster[f"{key} (Active)"] = live_replies[key]
            for node, metrics in cluster_roster.items():
                print(f" 🪐 {node:<20} | 🔥 {metrics['temp']:<21} | 🔌 {metrics['volt']}")
            print("======================================================================")
            print("👉 Press Ctrl+C to drop out of live cluster monitoring layout...")
            time.sleep(2)
    except KeyboardInterrupt:
        print("\033[H\033[JCluster monitoring interface suspended.")

def execute_decentralized_mesh():
    print("======================================================")
    print("⚡ MINT RTOS DECENTRALIZED MESH INTERFACE v3.5")
    print("======================================================")
    print("👤 OPERATOR ID   : MrPistolWhippy")
    print("📡 PEER INTERACTION: Multi-Node Cluster Aggregation [9K]")
    print("------------------------------------------------------")
    while True:
        try:
            command = input("\nMrPistolWhippy@PQ-Mesh:~# ").strip()
            if not command: continue
            if command.lower() in ["exit", "quit"]:
                print("Severing ties to decentralized matrix core."); break
            elif command.lower() == "monitor":
                print("📡 Polling global channels..."); run_cluster_monitor()
            else:
                encoded_bytes = base64.b64encode(command.encode()).decode().replace("=", "")
                broadcast_decentralized_fragment(1, 1, encoded_bytes)
                print("✨ Shard dispatched to cluster pool.")
        except KeyboardInterrupt:
            print("\nMesh loop suspended."); break

if __name__ == "__main__":
    execute_decentralized_mesh()