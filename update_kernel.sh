#!/bin/ash
set -e

# Overwrite with bare-metal raw pointer structure
cat << 'DATA_EOF' > z_os/src/archive.zig
// Ziggy OS Free-Mode Bare-Metal Kernel Node
// Hardcoded raw pointers for freestanding arm-none-eabi compilation

pub const HistoricalRecord = extern struct {
    item_id: [*:0]const u8,
    title: [*:0]const u8,
    author: [*:0]const u8,
    owner: [*:0]const u8,
    address: [*:0]const u8,
    postal_code: [*:0]const u8,
    reserve_nzd: u32,
    latitude_fixed: i32,
    longitude_fixed: i32,
    incident_date: [*:0]const u8,
    flight_path_convent: [*:0]const u8,
    flight_path_cemetery: [*:0]const u8,
    flight_path_exit: [*:0]const u8,
    color_profile_hex: u32,
};

// Export to linker symbol space for arm-none-eabi-ld reference
export const wingspread_archive linksection(".rodata") = HistoricalRecord{
    .item_id = "wingspread_1941_archive_001",
    .title = "Wingspread: The Pioneering of Aviation in New Zealand",
    .author = "Leo White",
    .owner = "Peter Dann King",
    .address = "76 Prospect Terrace, Mt Eden, Auckland",
    .postal_code = "S.2",
    .reserve_nzd = 150,
    .latitude_fixed = -368884,
    .longitude_fixed = 1747530,
    .incident_date = "1942-03-13",
    .flight_path_convent = "Home of the Good Shepherd (Waikowhai Rd, Mt Roskill)",
    .flight_path_cemetery = "Hillsborough Cemetery",
    .flight_path_exit = "Manukau Heads",
    .color_profile_hex = 0x0011E6D4,
};
DATA_EOF

cp z_os/src/archive.zig ishfiles/ziggy-kernel-os/src/archive.zig

echo "=== UPDATED FILES PRESENT ==="
cat z_os/src/archive.zig | grep -E 'extern struct|export const'
