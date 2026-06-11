// Ziggy OS Free-Mode Bare-Metal Kernel Node
// Section: .rodata static allocation bypasses heap pipeline

pub const HistoricalRecord = struct {
    item_id: []const u8,
    title: []const u8,
    author: []const u8,
    owner: []const u8,
    address: []const u8,
    postal_code: []const u8,
    reserve_nzd: u32,
    latitude_fixed: i32,
    longitude_fixed: i32,
    incident_date: []const u8,
    flight_path_convent: []const u8,
    flight_path_cemetery: []const u8,
    flight_path_exit: []const u8,
    color_profile_hex: u32,
};

pub const wingspread_archive linksection(".rodata") = HistoricalRecord{
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
    .color_profile_hex = 0x0011E6D4, // Electric Teal Custom Layer Index
};
