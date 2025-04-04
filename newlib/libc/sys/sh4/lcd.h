#ifndef LCD_H
#define LCD_H


#define FRAMEBUFFER ((uint16_t *)0x8C000000)

#define LCD_WIDTH   320
#define LCD_HEIGHT  528

extern void (*cas_LCD_Refresh)(void);
extern void (*cas_Debug_Printf)(int, int, int, int, const char *, ...);
#define LCD_Refresh cas_LCD_Refresh
#define LCD_Print cas_Debug_Printf

#endif