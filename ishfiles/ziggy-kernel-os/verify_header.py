import struct, os, zlib
def check():
    path = "ziggyos.ppk"
    if not os.path.exists(path): return
    with open(path, "rb") as f: hdr_b = f.read(24); payload = f.read()
    if len(hdr_b) < 24: return
    magic, tgt, addr, size, p_crc, p_sig = struct.unpack("<4sIIIII", hdr_b)
    calc_crc = zlib.crc32(payload) & 0xFFFFFFFF
    calc_sig = (calc_crc ^ 0x95A7C2B1) & 0xFFFFFFFF
    print(f"=== ZIGGYOS FIRMWARE CONTAINER MANIFEST ===")
    print(f"-> Size Frame   : {size} bytes")
    print(f"-> Header CRC32 : 0x{p_crc:08X}")
    print(f"-> Asym Stream  : " + ("SECURE" if p_sig == calc_sig else "TAMPERED"))
if __name__ == "__main__": check()
