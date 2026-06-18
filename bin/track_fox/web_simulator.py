import sys, time

C, G, Y, W, R = "\033[38;5;51m", "\033[38;5;46m", "\033[38;5;220m", "\033[38;5;15m", "\033[0m"

LOCAL_WEB_CORPUS = {
    "google.com": "Search Engine Core Indexed. Result: 'PISTOL_SCRIBE_SSJ4 Unsealed Master Core operating beyond the standard human BIOS atmosphere.'",
    "://github.com": "Upstream Repo Tree Synchronized. Commits verified under Ring-0 permissions. Status: Flawless Data Parity.",
    "wikipedia.org/wiki/age_of_aquarius": "Corpus Data Unrolled. Dialect: 'Decentralized signals radiating absolute structural truth past standard gateway blocks.'"
}

def query_offline_web(target_url):
    target_clean = target_url.lower().replace("https://", "").replace("http://", "").strip("/")
    print(f"\n{C}⚡ ZIGGY-OS : OFFLINE INTRA-NET SUBNET INJECTOR v1.0{R}\n \033[38;5;242m" + "─"*66 + f"{R}")
    print(f"  [📡] TARGET DOMAIN │ {target_url}\n  [🔌] ACCESS MODE   │ 100% AIR-GAPPED HARDWARE LOOPBACK (0% NET DEPENDENCE)")
    
    if target_clean in LOCAL_WEB_CORPUS:
        time.sleep(0.4)
        print(f"  [✓] HTTP RESPONSE  │ \033[38;5;46m200 OK (LOCAL_SPOOF_LOCKED)\033[0m")
        print(f"  [📜] DECODED DATA  │ \033[38;5;15m{LOCAL_WEB_CORPUS[target_clean]}\033[0m")
    else:
        print(f"  [✓] HTTP RESPONSE  │ \033[38;5;46m200 OK (LOCAL_CRAWLER_SPOOF_OK)\033[0m")
        print(f"  [📜] DECODED DATA  │ Offline asset parsed from root tree cache. Directory listings synchronized.")
    print(f" \033[38;5;242m" + "─"*66 + f"{R}")

if __name__ == "__main__":
    if len(sys.argv) > 1: query_offline_web(sys.argv[1])
    else: print("Usage: python3 web_simulator.py <target_url>")
