import sqlite3
c = sqlite3.connect("/root/archive.db").cursor()
nodes = [
    ("cryptography", "CRYPTO MATRIX TABLES", "Details evolutionary shift from AES/RSA to multi-dimensional Learning-With-Errors (LWE) post-quantum lattice algorithms."),
    ("protocols", "NETWORKING PROTOCOLS", "Maps structural layouts for core internet suites including TCP, UDP telemetry, and SSH cryptographic handshakes."),
    ("computing", "COMPUTING HISTORY ARCHIVE", "Traces milestones from Babbage, Turing machine concepts, to modern parallel FPGA hardware synthesis logic networks.")
]
for k, s, p in nodes: c.execute("INSERT OR IGNORE INTO offline_web_vault (keyword, summary, raw_text_payload) VALUES (?, ?, ?)", (k, s, p))
c.connection.commit(); c.connection.close()
print("[✓] Nodes fused.")
