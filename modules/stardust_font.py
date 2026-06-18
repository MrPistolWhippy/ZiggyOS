import sys
import random

# Compressed 5x5 pixel matrix font arrays for "ZIGGY"
GLYPHS = {
    'Z': ["#####", "   #/ ", "  # / ", " #   /", "#####"],
    'I': [" ### ", "  #  ", "  #  ", "  #  ", " ### "],
    'G': [" ####", "#    ", "#  ##", "#   #", " ####"],
    'Y': ["#   #", " # # ", "  #  ", "  #  ", "  #  "],
    ' ': ["     ", "     ", "     ", "     ", "     "]
}

STARDUST_SPARKS = ['+', '.', '*', '°', '·', ' ']

def render_stardust(text):
    text = text.upper()
    lines = ["" for _ in range(5)]
    
    # Build text layers
    for char in text:
        if char in GLYPHS:
            for i in range(5):
                lines[i] += GLYPHS[char][i] + "  "
        else:
            for i in range(5):
                lines[i] += GLYPHS[' '][i] + "  "

    print("\n" + "✧ " * 30 + "\n")
    
    # Process matrix rows and inject random environmental stardust
    for line in lines:
        rendered_row = ""
        for char in line:
            if char == '#':
                rendered_row += "★" # Core dense star matter
            elif char == '/' or char == ' ':
                # 35% chance to generate drifting stardust particles in empty tracks
                rendered_row += random.choice(STARDUST_SPARKS) if random.random() < 0.35 else " "
            else:
                rendered_row += char
        print(f"  {rendered_row}")
        
    print("\n" + " ✧" * 30 + "\n")

if __name__ == "__main__":
    target = "ZIGGY" if len(sys.argv) < 2 else " ".join(sys.argv[1:])
    render_stardust(target)
