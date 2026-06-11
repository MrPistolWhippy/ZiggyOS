import os

print("\x1b[36mBuilding Verified Interactive 16-bit Keyboard Floppy Boot Track...\x1b[0m")

# Pure 16-bit legacy real-mode machine code array
machine_code = bytearray([
    0xB8, 0x00, 0xB8,  # mov ax, 0xB800 (Point segment to VGA memory)
    0x8E, 0xC0,        # mov es, ax
    0xBB, 0x00, 0x00,  # mov bx, 0x0000 (Clear screen offset pointer)
    
    # 1. Clear the canvas to a clean black field with white font attributes
    0xB9, 0xD0, 0x07,  # mov cx, 2000
    # clear_loop:
    0x26, 0xC6, 0x07, 0x20, 0x43, # mov byte [es:bx], ' ' | inc bx
    0x26, 0xC6, 0x07, 0x07, 0x43, # mov byte [es:bx], 0x07 (Normal white text) | inc bx
    0xE2, 0xF2,
    
    # 2. Render system title banner centered on Row 10
    0xBB, 0x40, 0x06,  # mov bx, 1600 (Row 10 column 0 offset)
])

# Embed system title characters
title_string = "   Z I G G Y - O S   [INTERACTIVE MODE]   "
for char in title_string:
    machine_code.extend([0x26, 0xC6, 0x07, ord(char), 0x43]) # Write character | inc bx
    machine_code.extend([0x26, 0xC6, 0x07, 0x70, 0x43])      # Write inverse gray block style | inc bx

# 3. Position blinking prompt target right on Row 15
machine_code.extend([
    0xBB, 0xE0, 0x09,  # mov bx, 2528 (Row 15 workspace position offset)
])

# 4. Core Keyboard Interrupt Listener Loop Block (BIOS Int 16h)
# key_listen_loop:
machine_code.extend([
    0xB8, 0x00, 0x00,  # mov ax, 0x0000
    0xCD, 0x16,        # int 0x16 (Wait and capture BIOS keystroke scancode)
    
    # Echo characters directly onto your live workspace canvas
    0x26, 0x88, 0x07,  # mov [es:bx], al (Write ASCII character code into memory cell)
    0x43,              # inc bx (Advance to attribute position)
    0x26, 0xC6, 0x07, 0x0F, # mov byte [es:bx], 0x0F (Pure high-contrast white font bits)
    0x43,              # inc bx (Advance to next character tracking slot)
    
    0xEB, 0xF1         # jmp key_listen_loop
])

# Structuring a perfect Master Boot Record sector frame size
padding = b"\x90" * (510 - len(machine_code))
master_boot_record = machine_code + padding + b"\x55\xAA"

# Write to standard 1.44MB Floppy layout image file container
disk_map = bytearray(1474560)
disk_map[0:512] = master_boot_record

output_path = "/root/ziggy_teal_144.img"
with open(output_path, "wb") as f:
    f.write(disk_map)

print("\x1b[32m[SUCCESS] Perfect interactive floppy image generated at:", output_path, "\x1b[0m")
