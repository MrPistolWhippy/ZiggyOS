import sys
import time
import random

# Condensed 4-line matrix representation for "ZIGGY"
GLYPHS = {
    'Z': ["####", "  #/ ", " #/  ", "####"],
    'I': [" ###", "  # ", "  # ", " ###"],
    'G': [" ## ", "#   ", "# ##", " ## "],
    'Y': ["#  #", " ## ", "  # ", "  # "],
    ' ': ["    ", "    ", "    ", "    "]
}
SPARKS = ['+', '*', '.', ' ', ' ']

def run_compact_loop():
    frame = 0
    try:
        while True:
            frame += 1
            # Return cursor to home position and refresh layout
            sys.stdout.write("\033[H\033[J")
            sys.stdout.write(f"⚡ FRAME: {frame} | STATUS: OPTIMIZED ⚡\n\n")
            
            for row_idx in range(4):
                rendered_row = ""
                for char in "ZIGGY":
                    if char in GLYPHS:
                        for cell in GLYPHS[char][row_idx]:
                            if cell == '#':
                                rendered_row += "■"
                            else:
                                rendered_row += random.choice(SPARKS) if random.random() < 0.15 else " "
                sys.stdout.write(f"  {rendered_row}\n")
            
            sys.stdout.write(f"\nPress Ctrl+C to exit.\n")
            sys.stdout.flush()
            time.sleep(0.2)
    except KeyboardInterrupt:
        print("\n[+] Visualization loop terminated.")

if __name__ == "__main__":
    run_compact_loop()
