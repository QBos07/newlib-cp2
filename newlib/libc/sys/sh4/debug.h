#define DEBUG_CHAR_WIDTH 	8
#define DEBUG_CHAR_HEIGHT	12
#define DEBUG_LINE_HEIGHT	14

#define DEBUG_MAX_COLS    40
#define DEBUG_MAX_ROWS    37

#ifndef NO_CAS_DECLS
#include <stdint.h>
extern uintptr_t cas_fontbase;
#endif
#define DEBUG_FONTBASE    cas_fontbase

#define DEBUG_COLOR_OUT   0xFFFF

extern char (*debug_lines)[DEBUG_MAX_ROWS][DEBUG_MAX_COLS];