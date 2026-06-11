import os

print("\x1b[36mDeploying True Full-Screen Deep Neon Electric Teal Matrix...\x1b[0m")

machine_code = bytearray([
    0xFC,              # cld (Clear direction flag)
    0xB8, 0x00, 0xB8,  # mov ax, 0xB800 (Point to VGA memory segment base)
    0x8E, 0xC0,        # mov es, ax
    
    # --- STEP 1: INITIALIZE NEW DEEP PREMIUM NEON CANVASES ---
    0x31, 0xDF,        # xor di, di
    0xB9, 0xD0, 0x07,  # mov cx, 2000
    0xB8, 0x20, 0x1B,  # mov ax, 0x1B20 (AL = ' ' space, AH = 0x1B premium dark neon teal backdrop)
    0xF3, 0xAB,        # rep stosw (Flood canvas instantly with the deep palette)
    
    # --- STEP 2: FIX LOOP POINTER POSITION (PULLED OUTSIDE TRAP) ---
    0x31, 0xDB,        # xor bx, bx (Initialize pointer to 0 out here safely!)
    
    # rain_matrix_cycle_start:
    0xB9, 0x1E, 0x00,  # mov cx, 30 (Draw 30 cascading rain nodes per frame refresh)
    
    # render_droplets:
    0xE4, 0x40,        # in al, 0x40 (Read random hardware oscillator counter ticks)
    0x24, 0x3F,        # and al, 0x3F (Limit boundaries)
    0x04, 0x21,        # add al, 0x21 (Translate bytes directly to sharp ASCII matrix glyphs)
    
    0x26, 0x88, 0x07,  # mov [es:bx], al (Stitch character glyph byte into current video cell)
    0x43,              # inc bx (Advance to attribute byte position)
    0x26, 0xC6, 0x07, 0x1B, # mov byte [es:bx], 0x1B (Keep matching the gorgeous deep neon teal color flags)
    0x43,              # inc bx (Advance to next character tracking slot)
    
    # Bound Check: If pointer reaches end of full screen (4000 bytes), wrap back to top row
    0x81, 0xFB, 0xA0, 0x0F, # cmp bx, 4000
    0x72, 0x02,        # jb skip_reset
    0x31, 0xDB,        # xor bx, bx (Wrap pointer back to top cell cleanly)
    
    # skip_reset:
    0xE2, 0xE5,        # loop render_droplets
    
    # --- STEP 3: SCREEN FRAME REFRESH SPEED TIMING ---
    0x51,              # push cx
    0xBE, 0x30, 0x00,  # mov si, 48 (High-speed matrix waterfall optimization tracking flag)
    # delay_outer:
    0x31, 0xC9,        # xor cx, cx
    # delay_inner:
    0x41,              # inc cx
    0x75, 0xFD,        # jnz delay_inner
    0x4E,              # dec si
    0x75, 0xF6,        # jnz delay_outer
    0x59,              # pop cx
    
    0xEB, 0xCD         # jmp rain_matrix_cycle_start (Loop cascade animation infinitely down screen!)
])

# Seal the Master Boot Record structure layout
padding = b"\x90" * (510 - len(machine_code))
master_boot_record = machine_code + padding + b"\x55\xAA"

disk_map = bytearray(1474560)
disk_map[0:512] = master_boot_record

# Overwrite out to a brand-new filename to permanently smash UTM's disk cache
output_path = "/root/neon_matrix.img"
with open(output_path, "wb") as f:
    f.write(disk_map)

print("\x1b[32m[SUCCESS] Full-screen Neon Electric Teal master disk baked at:", output_path, "\x1b[0m")
