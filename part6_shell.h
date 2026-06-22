#pragma once
#include "part5_oaep.h"

#define CMD_MAX_LEN 64

int z_shell_execute(const char* input, char* output_resp) {
    if (strncmp(input, "help", 4) == 0) {
        strcpy(output_resp, "Ziggy-OS Mini-Shell. Available vectors: help, sysinfo, clear.");
        return ZIGGY_OK;
    } else if (strncmp(input, "sysinfo", 7) == 0) {
        strcpy(output_resp, "Kernel Status: ELECTRIC TEAL. Cores: 4 SMP. MMU: Sv39 Enabled.");
        return ZIGGY_OK;
    } else {
        strcpy(output_resp, "Execution Error: Unknown system vector token.");
        return ZIGGY_ERR;
    }
}
