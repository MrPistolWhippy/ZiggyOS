import os

print("\x1b[36mAssembling Final Lumos OS Spirit Bomb Microkernel Sector Array...\x1b[0m")

# Production-grade 16-bit real-mode assembly sequence
machine_code = bytearray([
    0xB8, 0x00, 0xB8,  # mov ax, 0xB800 (Lock video memory segment register base)
    0x8E, 0xC0,        # mov es, ax
    0xBB, 0x00, 0x00,  # mov bx, 0x0000 (Clear buffer tracking pointer)
    
    # 1. Flash clean dark text workspace canvas layout
    0xB9, 0xD0, 0x07,  # mov cx, 2000
    # init_canvas_loop:
    0x26, 0xC6, 0x07, 0x20, 0x43, # mov byte [es:bx], ' ' | inc bx
    0x26, 0xC6, 0x07, 0x07, 0x43, # mov byte [es:bx], 0x07 (Classic white text bits) | inc bx
    0xE2, 0xF2,
    
    # 2. Render centered header label strings on row 10
    0xBB, 0x40, 0x06,  # mov bx, 1600
])

title = "   Z I G G Y - O S   [LUMOS MATRIX MODE]   "
for char in title:
    machine_code.extend([0x26, 0xC6, 0x07, ord(char), 0x43])
    machine_code.extend([0x26, 0xC6, 0x07, 0x70, 0x43]) # High-contrast inverse grid block style

# Initialize input prompt on row 15
machine_code.extend([
    0xBB, 0xE0, 0x09,  # mov bx, 2528 (Row 15 workspace position offset)
])

# 3. Interactive Keyboard Scan Routing Loops
# await_keypress:
machine_code.extend([
    0xB8, 0x00, 0x00,  # mov ax, 0x0000
    0xCD, 0x16,        # int 0x16 (Trigger hardware keyboard interrupt hook)
    
    # Intercept Rule: If user hits the ENTER/RETURN key (0x0D), ignite the Spirit Bomb!
    0x3C, 0x0D,        # cmp al, 0x0D
    0x74, 0x0C,        # je ignite_spirit_bomb
    
    # Standard Character Echo Path
    0x26, 0x88, 0x07, 0x43,      # mov [es:bx], al | inc bx
    0x26, 0xC6, 0x07, 0x0F, 0x43, # mov byte [es:bx], 0x0F (Crisp pure white font attribute) | inc bx
    0xEB, 0xEC,        # jmp await_keypress
    
    # 4. The Spirit Bomb: Explode full screen into animated cascading Electric Teal text loops
    # ignite_spirit_bomb:
    0x31, 0xBB,        # xor bx, bx (Reset block index pointer to zero)
    # bomb_loop:
    0xB0, 0x43,        # mov al, 'C' (Custom matrix cascading text block element glyph)
    0x26, 0x88, 0x07, 0x43, # mov [es:bx], al | inc bx
    0xB0, 0x3B,        # mov al, 0x3B (Electric Teal background vector configuration matrix)
    0x26, 0x88, 0x07, 0x43, # mov [es:bx], al | inc bx
    
    # Add a high-speed hardware cycling delay loop to animate the rows smoothly
    0x51,              # push cx
    0xB9, 0x00, 0x10,  # mov cx, 4096
    # delay_loop:
    0x4A, 0xE2, 0xFD,  # dec dx | loop delay_loop
    0x59,              # pop cx
    
    0x81, 0xFB, 0xA0, 0x0F, # cmp bx, 4000 (Check if full screen video memory is saturated)
    0x72, 0xE3,        # jb bomb_loop
    0xEB, 0xDF         # jmp await_keypress (Bounce back to input mode once graphics stabilize)
])

# Structure and lock Master Boot Record parameters
padding = b"\x90" * (510 - len(machine_code))
master_boot_record = machine_code + padding + b"\x55\xAA"

disk_map = bytearray(1474560)
disk_map[0:512] = master_boot_record

output_path = "/root/ziggy_teal_144.img"
with open(output_path, "wb") as f:
    f.write(disk_map)

print("\n\x1b[32m[SPIRIT BOMB LOCKED] High-speed real-mode patch compiled to:", output_path, "\x1b[0m")
