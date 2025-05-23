#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#if __STDC_VERSION__ < 202311L //C23 - defines nullptr and booleans
#include <stdbool.h>
#define nullptr ((void *)NULL)
#endif

#include "impl.h"

#include "cas_file.h"
#include "cas_memory.h"
#include "debug.h"
#include "lcd.h"

#define REGISTER_SYMBOL(name, sym) \
    typeof(sym) sym = { .s = { (void *)(0xBEEF0001 + __LINE__ * 2), #name } };

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

static symbol_entry * const symbols[] = {
    &cas_LCD_Refresh.s,
    &cas_Debug_Printf.s,

    &cas_fontbase.s,

    &cas_open.s,
    &cas_close.s,
    &cas_read.s,
    &cas_fstat.s,
    &cas_mkdir.s,
    &cas_lseek.s,
    &cas_write.s,
    &cas_stat.s,

    &cas_malloc.s,
    &cas_free.s,
};

#define SAFE_GUARD ((const char *)0x814fffe0)
#define SAFE_GUARD_SIZE 16

static bool checkSafeGuard(const char * const other) {
    return strncmp(SAFE_GUARD, other, SAFE_GUARD_SIZE) == 0;
}

static const void ** findFunc(const char * const name) {
    for (symbol_entry * const * entry = symbols; entry < symbols + sizeof(symbols) / sizeof(entry); entry++)
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
#ifndef __clang__
#pragma GCC diagnostic ignored "-Wprio-ctor-dtor"
#endif
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

__attribute__((weak)) typeof(debug_lines) debug_lines = nullptr;
static bool debug_lines_malloced = false;

static __attribute__((constructor(3))) void init_debug(void) {
    if (debug_lines != nullptr)
        return;
    debug_lines = malloc(sizeof(*debug_lines));
    debug_lines_malloced = true;
}

static __attribute__((destructor)) void free_debug(void) {
    if (!debug_lines_malloced)
        return;
    free(debug_lines);
    debug_lines = nullptr;
}
#pragma GCC diagnostic pop
