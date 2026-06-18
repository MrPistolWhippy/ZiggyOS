#!/usr/bin/env python3
import time, secrets, os
from datetime import datetime
os.makedirs("/root/logs", exist_ok=True)
try:
    ts = datetime.now().strftime('%H:%M:%S')
    with open("/root/logs/network_sniff.log", "a") as f:
        f.write(f"[{ts}] RF_WAVE | RAW_HEX: 4745545F5A49474759_{secrets.token_hex(6).upper()} | IQ_BLOCK\n")
except: pass
