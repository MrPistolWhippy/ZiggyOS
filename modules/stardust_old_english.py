import sys
import time
import os
import random

# Old English stylistic 5x7 dense matrix representations for "ZIGGY"
GLYPHS = {
    'Z': ["#######", "    ## ", "   ##  ", "  ##   ", " ##    ", "##     ", "#######"],
    'I': ["#######", "  ###  ", "  ###  ", "  ###  ", "  ###  ", "  ###  ", "#######"],
    'G': [" ##### ", "##   ##", "##     ", "## ####", "##   ##", "##   ##", " ##### "],
    'Y': ["##   ##", "##   ##", " ## ## ", "  ###  ", "  ###  ", "  ###  ", "  ###  "],
    ' ': ["       ", "       ", "       ", "       ", "       ", "       ", "       "]
}

SPARKS = ['+', '*', '.', '°', '✧', '✨', ' ', ' ', ' ']

def draw_frame(text, frame_tick):
    lines = ["" for _ in range(7)]
    for char in text.upper():
        if char in GLYPHS:
            for i in range(7):
                lines[i] += GLYPHS[char][i] + "   "
        else:
            for i in range(7):
                lines[i] += GLYPHS[' '][i] + "   "

    # Clear terminal space using classic escape codes
    sys.stdout.write("\033[H\033[J")
    sys.stdout.write(f"⚡ [ANIMATED GLYPH LOOP] FRAME: {frame_tick} | STATUS: RUNNING ⚡\n\n")
    
    for row in lines:
        rendered_row = ""
        for cell in row:
            if cell == '#':
                # Dense letter core structure
                rendered_row += "𝖅" if frame_tick % 2 == 0 else "time"
                rendered_row = rendered_row[:-4] + "𝖅" # Keep length correct
            else:
                # Shimmering stardust background layers with randomized frequency
                rendered_row += random.choice(SPARKS) if random.random() < 0.25 else " "
        sys.stdout.write(f"  {rendered_row}\n")
    
    sys.stdout.write(f"\nPress Ctrl+C to break out of the visualization loop.\n")
    sys.stdout.flush()

if __name__ == "__main__":
    target_word = "ZIGGY" if len(sys.argv) < 2 else sys.argv[1]
    tick = 0
    try:
        while True:
            draw_frame(target_word, tick)
            tick += 1
            time.sleep(0.15) # Emulate a traditional 6.6 FPS animated image sequence
    except KeyboardInterrupt:
        print("\n[+] Visual loop closed cleanly. System returned to standard console.")
