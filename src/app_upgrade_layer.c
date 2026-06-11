#include <stdint.h>

#define SECTOR_SIZE 512
#define FAT12_MAX_FILES 4

typedef struct {
    char name[8];
    char ext[3];
    uint32_t size;
    uint32_t start_sector;
} __attribute__((packed)) DirectoryEntry;

// 1.44MB Virtual Floppy Storage Vector Simulation Matrix
static uint8_t virtual_floppy_sectors[FAT12_MAX_FILES * SECTOR_SIZE] = {0};
static DirectoryEntry root_dir[FAT12_MAX_FILES];

extern void print(const char* text);
extern void app_set_system_theme(char s);

void init_fat12_storage(void) {
    // File 1: Ground Station Configuration Metadata Profile
    for(int i=0; i<8; i++) root_dir[0].name[i] = "CONFIG  "[i];
    root_dir[0].size = 32;
    root_dir[0].start_sector = 0;
    
    // Inject the tactical 144.777 telemetry target packet directly into file storage sector
    const char* config_payload = "FREQ=144.777nNODE=AKL_RECONnSTATUS=LOCK";
    for(int i=0; config_payload[i] != '0'; i++) {
        virtual_floppy_sectors[i] = config_payload[i];
    }
}

void app_fat12_cat(const char* filename) {
    if (filename[0] == 'c') {
        print("n--- Reading /CONFIG.TXT from FAT12 Sector 0 ---n");
        print((const char*)virtual_floppy_sectors);
        print("n---------------------------------------------n");
    } else {
        print("n[-] Error: File not found in FAT12 directory indexing.n");
    }
}

// -------------------------------------------------------------
// FEATURE 2: DIRECT HARDWARE VGA PORT ARCHITECTURE CONTROLLER
// -------------------------------------------------------------
void app_set_vga_color_scheme(char color_byte) {
    // Hardware I/O ports for standard VGA Attribute and CRTC controllers
    // In bare-metal x86, we write out to physical registers using instructions like outb
    // For our sandboxed multi-arch tracking layer, we update the VGA status indicators
    if (color_byte == 'm') {
        print("n[VGA Controller]: Port 0x03C0 latched. Theme -> MATRIX GREENn");
    } else if (color_byte == 'a') {
        print("n[VGA Controller]: Port 0x03C0 latched. Theme -> RETRO AMBERn");
    } else {
        print("n[VGA Controller]: Port 0x03C0 reset to standard gray console profile.n");
    }
}

// -------------------------------------------------------------
// COMBINED FEATURE: LIVE TACTICAL RADIO DECODER ENGINE (144.777 MHz)
// -------------------------------------------------------------
void app_run_tactical_decoder(void) {
    print("n==================================================n");
    print("📡 TARGET ACQUIRED: MONITORING PACKET STREAM (144.777 MHz)n");
    print("==================================================n");
    
    // Simulate interactive parsing of signal nodes matching Auckland track parameters
    print("[DATA DECODER]: Tracking Vector Lock Stable.n");
    print("  Latitude  : 36.8485 S (Auckland Grid Coordinate)n");
    print("  Longitude : 174.7633 En");
    print("  Telemetry : Tracking target wingspread_1941_archive_001n");
    print("==================================================n");
}
