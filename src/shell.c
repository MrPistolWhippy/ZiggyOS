/* ==============================================================================
 *          ZIGGY-OS USER LAND: ENVIRONMENT VARIABLES & GREP PARSER
 * ============================================================================== */
#include <stdint.h>
#include <stddef.h>

extern void uart_puts(const char *s);
void uart_putc(char c);

#define VFS_MAX_NAME 32
#define MAX_ENV_VARS 8

/* --- 1. ENVIRONMENT VARIABLES STORAGE LAYOUT --- */
typedef struct {
    char key[16];
    char value[32];
    uint8_t active;
} EnvVar_t;

static EnvVar_t env_table[MAX_ENV_VARS];

/* String comparison utility */
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

/* Substring finder for grep implementation */
const char *strstr(const char *haystack, const char *needle) {
    if (!*needle) return haystack;
    for (; *haystack; haystack++) {
        if (*haystack == *needle) {
            const char *h = haystack;
            const char *n = needle;
            while (*h && *n && *h == *n) {
                h++;
                n++;
            }
            if (!*n) return haystack;
        }
    }
    return NULL;
}

/* --- 2. ENVIRONMENT ENGINE SUBROUTINES --- */
void ziggy_setenv(const char *key, const char *val) {
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (env_table[i].active && strcmp(env_table[i].key, key) == 0) {
            int j = 0; while (val[j] && j < 31) { env_table[i].value[j] = val[j]; j++; }
            env_table[i].value[j] = '\0';
            return;
        }
    }
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (!env_table[i].active) {
            int j = 0; while (key[j] && j < 15) { env_table[i].key[j] = key[j]; j++; }
            env_table[i].key[j] = '\0';
            j = 0; while (val[j] && j < 31) { env_table[i].value[j] = val[j]; j++; }
            env_table[i].value[j] = '\0';
            env_table[i].active = 1;
            uart_puts("[✓] Env Variable Stored.\n");
            return;
        }
    }
}

const char *ziggy_getenv(const char *key) {
    for (int i = 0; i < MAX_ENV_VARS; i++) {
        if (env_table[i].active && strcmp(env_table[i].key, key) == 0) {
            return env_table[i].value;
        }
    }
    return "NOT_SET";
}

/* --- 3. DYNAMIC GREP TEXT PARSER FILTER --- */
void ziggy_grep(const char *pattern, const char *text_target) {
    uart_puts("\n[GREP MATCH FILTER RUNNING]:\n");
    if (strstr(text_target, pattern)) {
        uart_puts("  --> FOUND MATCH: ");
        uart_puts(text_target);
        uart_puts("\n");
    } else {
        uart_puts("  --> No matches located for specified pattern.\n");
    }
}

/* --- 4. MASTER SHELL EXECUTION CONSOLE INTERFACES --- */
void ziggy_shell_run(const char *cmd_buffer) {
    if (strcmp(cmd_buffer, "help") == 0) {
        uart_puts("\n--- ZIGGY-OS ADVANCED USERLAND TASKS ---");
        uart_puts("\n  env   - Mock show environmental targets");
        uart_puts("\n  grep  - Filter text string pattern logs\n");
    }
    else if (strcmp(cmd_buffer, "env") == 0) {
        uart_puts("\nActive Execution Path: ");
        uart_puts(ziggy_getenv("PATH"));
        uart_puts("\n");
    }
}
