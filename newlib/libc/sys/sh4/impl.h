#pragma once

typedef struct {
    const void * ptr;
    const char * const name;
} symbol_entry;

#define CAS_FUN(prefix, name, ...) \
extern union { \
    prefix (*f) __VA_ARGS__; \
    symbol_entry s; \
} name;
#define CAS_VAR(prefix, name, ...) \
extern union { \
    prefix v __VA_ARGS__; \
    symbol_entry s; \
} name;
