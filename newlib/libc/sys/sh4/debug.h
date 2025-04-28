#pragma once

#include <stdint.h>

#include "impl.h"

#define DEBUG_CHAR_WIDTH 	8
#define DEBUG_CHAR_HEIGHT	12
#define DEBUG_LINE_HEIGHT	14

#define DEBUG_MAX_COLS    40
#define DEBUG_MAX_ROWS    37

CAS_VAR(uintptr_t, cas_fontbase)
#define DEBUG_FONTBASE    cas_fontbase.v

#define DEBUG_COLOR_OUT   0xFFFF

extern char (*debug_lines)[DEBUG_MAX_ROWS][DEBUG_MAX_COLS];