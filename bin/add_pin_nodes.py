import sqlite3
c = sqlite3.connect("/root/archive.db").cursor()
nodes = [
    ("pad_e12", "TSMC_28NM CLOCK PIN", "PAD_E12 maps directly to the primary 125 MHz crystal oscillator lines with strict 8.00ns propagation caps."),
    ("pad_m22", "TSMC_28NM WARNING LED", "PAD_M22 drives the direct physical status warning LED pin with zero-latency hardware override pathways."),
    ("pad_p04", "TSMC_28NM CRYPTO LATCH", "PAD_P04 acts as the dedicated hardware register gate output line for Post-Quantum vector validation flags.")
]
for k, s, p in nodes: c.execute("INSERT OR IGNORE INTO offline_web_vault (keyword, summary, raw_text_payload) VALUES (?, ?, ?)", (k, s, p))
c.connection.commit(); c.connection.close()
print("[✓] Silicon blueprint nodes fused.")
