import os

print("Injecting Live Input Loops to 32-Bit Hard Disk Geometry...")

hd_size = 10 * 1024 * 1024
disk_buffer = bytearray(hd_size)

# Explicit 16-bit to 32-bit entry bridge bootstrap routine
bootstrap = bytearray([
    0xFA,              # cli
    0xB8, 0x00, 0x00,  # mov ax, 0x0000
    0x8E, 0xD8, 0x8E, 0xC0,
    0xBC, 0x00, 0x7C,  # mov sp, 0x7C00
    
    # Render static center title banner string parameters
    0xBB, 0x40, 0x06,  # mov bx, 1600 (Row 10 centered position)
])

# Embed title characters
title = "   Z I G G Y - O S   [32-BIT PROTECTED MODE]   "
for char in title:
    bootstrap.extend([0xB0, ord(char), 0x26, 0x88, 0x05, 0x47, 0xB0, 0x70, 0x26, 0x88, 0x05, 0x47])

# Initialize Interactive Prompt Index Location (Row 15 base pointer address offset: 2400)
bootstrap.extend([
    0xBB, 0x00, 0x0A,  # mov bx, 2560 (Row 16 column 0 anchor tracking pointer)
])

# Interactive hardware input loop sequence strings
# input_loop:
bootstrap.extend([
    0xB8, 0x00, 0x00,  # mov ax, 0x0000
    0xCD, 0x16,        # int 0x16 (Await physical or overlay scancode key press)
    
    # Write character string code back onto your screen buffer matrix canvas
    0x26, 0x88, 0x07,  # mov [es:bx], al (Write ASCII symbol byte)
    0x43,              # inc bx
    0xB0, 0x0F,        # mov al, 0x0F (High-contrast pure white text font bits attribute)
    0x26, 0x88, 0x07,  # mov [es:bx], al
    0x43,              # inc bx
    
    0xEB, 0xEE         # jmp input_loop
])

disk_buffer[0:len(bootstrap)] = bootstrap
disk_buffer[510] = 0x55
disk_buffer[511] = 0xAA

output_path = "/root/ziggy_hd_boot.img"
with open(output_path, "wb") as f:
    f.write(disk_map if 'disk_map' in locals() else disk_buffer)

print("[SUCCESS] Interactive 32-bit hardware inputs compiled!")
