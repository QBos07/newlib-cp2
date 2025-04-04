#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    const void * ptr;
    const char * const name;
} symbol_entry;

#define REGISTER_SYMBOL(name, sym) \
    symbol_entry sym = { (void *)(0xBEEF0001 + __LINE__ * 2), #name };

REGISTER_SYMBOL(LCD_Refresh, cas_LCD_Refresh)
REGISTER_SYMBOL(Debug_Printf, cas_Debug_Printf)

REGISTER_SYMBOL(DEBUG_FONTBASE, cas_fontbase)

REGISTER_SYMBOL(File_Open, cas_open)
REGISTER_SYMBOL(File_Close, cas_close)
REGISTER_SYMBOL(File_Read, cas_read)
REGISTER_SYMBOL(File_Fstat, cas_fstat)
REGISTER_SYMBOL(File_Mkdir, cas_mkdir)
REGISTER_SYMBOL(File_Lseek, cas_lseek)
REGISTER_SYMBOL(File_Write, cas_write)
REGISTER_SYMBOL(File_Stat, cas_stat)

REGISTER_SYMBOL(Mem_Malloc, cas_malloc)
REGISTER_SYMBOL(Mem_Free, cas_free)

static symbol_entry *symbols[] = {
    &cas_LCD_Refresh,
    &cas_Debug_Printf,

    &cas_fontbase,

    &cas_open,
    &cas_close,
    &cas_read,
    &cas_fstat,
    &cas_mkdir,
    &cas_lseek,
    &cas_write,
    &cas_stat,

    &cas_malloc,
    &cas_free,
};

#define SAFE_GUARD ((const char *)0x814fffe0)
#define SAFE_GUARD_SIZE 16

#define nullptr ((void *)0x00000000)

static bool checkSafeGuard(const char * const other) {
    return strncmp(SAFE_GUARD, other, SAFE_GUARD_SIZE) == 0;
}

static const void ** findFunc(const char * const name) {
    for (symbol_entry **entry = symbols; entry < symbols + sizeof(symbols) / sizeof(entry); entry++)
        if (strcmp((*entry)->name, name) == 0)
            return &(*entry)->ptr;

    return nullptr;
}

bool ___relink(const uint8_t * const mapping, size_t len) {
    const uint8_t *ptr = mapping;

    {
        const char * guard = (void *)ptr;
        if (!checkSafeGuard(guard))
            return false;

        ptr += strlen(guard) + 1;
    }

    while ((size_t)(ptr - mapping) < len - 4) {
        const char * const str = (void *)(ptr);
        size_t const len = strlen(str);
        
        ptr += len + 1;

        uintptr_t val = 0;
        for(size_t i = 0; i < sizeof(val); i++) {
            val <<= 8;
            val |= ptr[i];
        }

        ptr += sizeof(val);

        const void ** func = findFunc(str);
        if (func)
            *func = (const void *)(val);
    }

    return true;
}

__attribute__((weak)) char ** environ;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wprio-ctor-dtor"
__attribute__((constructor(0))) void ___relocate_hhk(void) {
    char * env_var = getenv("HHK_SYMBOL_TABLE");
    if (env_var == nullptr)
        return;
    void * mapping = (void *)strtoul(env_var, nullptr, 16);
    if (mapping == nullptr)
        return;

    char * len_env_var = getenv("HHK_SYMBOL_TABLE_LEN");
    size_t len = strtoul(len_env_var, nullptr, 16);
    if (len == 0)
        return;

    if (!___relink(mapping, len))
        return;
}
#pragma GCC diagnostic pop