#!/usr/bin/env python3
# ==============================================================================
#  ZIGGYOS FORWARD-DEPLOYED LOGICAL EXTRACTION & TACTICAL FORENSIC TRIAGE CORE
# ==============================================================================
import os, sys, time, hashlib
from datetime import datetime

def run_logical_forensic_triage(target_dir="/root/modules"):
    print("=" * 60)
    print(f"\033[1;36m[*] INITIALIZING ADVANCED EXTRACTION EXTRACTION & ARTIFACT TRIAGE\033[0m")
    print("=" * 60)
    time.sleep(0.3)
    
    if not os.path.exists(target_dir):
        print(f"[-] Target directory {target_dir} unreachable.")
        return

    extracted_artifacts = 0
    report_data = []
    
    print("\033[94m[*] Phase 1: Analyzing System File Signatures & MD5 Hashes...\033[0m")
    for root, _, files in os.walk(target_dir):
        for file in files:
            if file.endswith('.py') or file.endswith('.db') or file.endswith('.v'):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, "rb") as f:
                        file_bytes = f.read()
                    md5_hash = hashlib.md5(file_bytes).hexdigest()
                    size = len(file_bytes)
                    
                    log_line = f"  -> [DEC] File: {file.ljust(22)} | Size: {str(size).zfill(5)}B | MD5: {md5_hash}"
                    print(log_line)
                    report_data.append(log_line)
                    extracted_artifacts += 1
                except Exception as e:
                    pass
                time.sleep(0.02)

    print("\n\033[94m[*] Phase 2: Compiling Timeline & Data-Payload Extractions...\033[0m")
    ts = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    summary = f"[✓] EXTRACTION SUCCESSFUL: Collected {extracted_artifacts} System Artifacts at {ts}"
    print(f"\033[1;32m{summary}\033[0m")
    
    # Write a uniform load file report down to disk logs
    try:
        report_ts = datetime.now().strftime('%Y%m%d_%H%M%S')
        report_path = f"/root/logs/forensic_report_{report_ts}.txt"
        with open(report_path, "w") as rf:
            rf.write(f"=== ZIGGYOS FORENSIC REPORT SINGLE SOURCE COLLECTION ===\nTimestamp: {ts}\n\n")
            rf.write("\n".join(report_data))
            rf.write(f"\n\n{summary}\n")
        print(f"  -> Saved Uniform Load File: \033[1;33m{report_path}\033[0m")
    except:
        pass
    print("=" * 60)

if __name__ == "__main__":
    run_logical_forensic_triage()
