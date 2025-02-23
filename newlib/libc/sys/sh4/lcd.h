#ifndef LCD_H
#define LCD_H


#define FRAMEBUFFER ((uint16_t *)0x8C000000)

#define LCD_WIDTH   320
#define LCD_HEIGHT  528

#define LCD_Refresh ((void (*)(void))0x800373AC)
#define LCD_Print   ((void (*)(int, int, int, int, const char *, ...))8002DBB0)

#endif