
import time
def show_hex():
    print("=== 👁️ CORE HEX MATRIX VISUALISER (BRRRRPPP!) ===")
    raw_bytes = [0x5A, 0x49, 0x47, 0x47, 0x59, 0x5F, 0x4F, 0x53, 0x99, 0xAA, 0xDE, 0xAD, 0xC0, 0xDE, 0x77, 0x77]
    print("  Address    00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   ASCII")
    print("  0x66600000 " + " ".join(f"{b:02X}" for b in raw_bytes) + "   ZIGGY_OS..DEADCODE")
if __name__ == "__main__": show_hex()
