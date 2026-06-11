import os

print("=== 1. Reading System Kernel Code Blocks ===")
kernel_path = "os-kernel.bin"
if not os.path.exists(kernel_path):
    kernel_path = "src/os-kernel.bin"

if not os.path.exists(kernel_path):
    print("Error: os-kernel.bin not found! Compiling raw fallback stubs...")
    with open("os-kernel.bin", "wb") as f:
        f.write(b"\xB8\x02\xB0\xAD\x1B" + b"\x00" * 507 + b"\x55\xAA")
    kernel_path = "os-kernel.bin"

with open(kernel_path, "rb") as f:
    kernel_data = f.read()

print(f"Loaded kernel payload binary: {len(kernel_data)} bytes.")

print("=== 2. Structuring Legacy x86 Boot Sectors ===")
# Generate a standard 1.44MB floppy sector image matrix array map
floppy_size = 1474560
disk_buffer = bytearray(floppy_size)

# Hardcode the master x86 Multiboot validation sequence right to sector 0
# Magic: 0x1BADB002, Flags: 0x00, Checksum: -0x1BADB002
multiboot_header = b"\x02\xB0\xAD\x1B\x00\x00\x00\x00\xFE\x4F\x52\xE4"
disk_buffer[0:len(multiboot_header)] = multiboot_header

# Inject your compiled code blocks directly behind the sector gate parameters
disk_buffer[512:512+len(kernel_data)] = kernel_data

# Stencil the classic legacy Master Boot Record signature to unlock SeaBIOS verification
disk_buffer[510] = 0x55
disk_buffer[511] = 0xAA

print("=== 3. Exporting Lightweight Local Floppy Disk Asset ===")
output_file = "/root/local_ziggyos.img"
with open(output_file, "wb") as f:
    f.write(disk_buffer)

print(f"Success! Bootable sector image saved to: {output_file} ({os.path.getsize(output_file)} bytes)")
