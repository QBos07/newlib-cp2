#pragma once

#include <stdint.h>

#include "impl.h"

#define FRAMEBUFFER ((uint16_t *)0x8C000000)

#define LCD_WIDTH   320
#define LCD_HEIGHT  528

CAS_FUN(void,cas_LCD_Refresh,(void))
CAS_FUN(void,cas_Debug_Printf,(int, int, int, int, const char *, ...))
#define LCD_Refresh cas_LCD_Refresh.f
#define LCD_Print cas_Debug_Printf.f
