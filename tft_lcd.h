#ifndef _TFTLCD_H_INCLUDED
#define _TFTLCD_H_INCLUDED

#include <avr/io.h>

#define LCD_Width  240
#define LCD_Height 320

#define DC_PIN  PB2 // Pin 16
#define CS_PIN  PB1 // Pin 15

#define LCD_PORTB  (1 << DC_PIN) | (1 << CS_PIN)

// LCD Macros
#define LCD_DC_Command   PORTB |= (1 << DC_PIN)
#define LCD_DC_Data      PORTB &= ~(1 << DC_PIN)
#define LCD_CS_Active    PORTB |= (1 << CS_PIN)
#define LCD_CS_Negate    PORTB &= ~(1 << CS_PIN)

// TFT LCD Registers
#define ILI9341_SOFTRESET       0x01  // Software Reset
#define ILI9341_SLEEPIN         0x10  // Enter Sleep Mode
#define ILI9341_SLEEPOUT        0x11  // Exit Sleep Mode
#define ILI9341_NORMALDISP      0x13  // Normal Display Mode ON
#define ILI9341_INVERTOFF       0x20  // Display Inversion OFF
#define ILI9341_INVERTON        0x21  // Display Inversion ON
#define ILI9341_GAMMASET        0x26  // Gamma Set
#define ILI9341_DISPLAYOFF      0x28  // Display OFF
#define ILI9341_DISPLAYON       0x29  // Display ON
#define ILI9341_COLADDRSET      0x2A  // Column Address Set
#define ILI9341_PAGEADDRSET     0x2B  // Page Address Set
#define ILI9341_MEMORYWRITE     0x2C  // Memory Write
#define ILI9341_PIXELFORMAT     0x3A  // Pixel Format Set
#define ILI9341_FRAMECONTROL    0xB1  // Frame Rate Control (In Normal Mode / Full Colors)
#define ILI9341_DISPLAYFUNC     0xB6  // Display Function Control
#define ILI9341_ENTRYMODE       0xB7  // Entry Mode
#define ILI9341_POWERCONTROL1   0xC0  // Power Control 1
#define ILI9341_POWERCONTROL2   0xC1  // Power control 2
#define ILI9341_VCOMCONTROL1    0xC5  // VCOM Control 1
#define ILI9341_VCOMCONTROL2    0xC7  // VCOM Control 2
#define ILI9341_MEMCONTROL      0x36  // Memory Access Control

#define ILI9341_MADCTL_MY       0x80  // Memory Access (Bottom to Top)
#define ILI9341_MADCTL_MX       0x40  // Memory Access (Right to Left)
#define ILI9341_MADCTL_MV       0x20  // Memory Access (Reverse Mode)
#define ILI9341_MADCTL_ML       0x10  // LCD Refresh (Bottom to Top)
#define ILI9341_MADCTL_RGB      0x00  // Set RGB Pixel Order
#define ILI9341_MADCTL_BGR      0x08  // Set BGR Pixel Order
#define ILI9341_MADCTL_MH       0x04  // LCD Refresh (Right to Left)

// Colors
#define LCD_BLACK       0x0000  //   0,   0,   0
#define LCD_NAVY        0x000F  //   0,   0, 123
#define LCD_DARKGREEN   0x03E0  //   0, 125,   0
#define LCD_DARKCYAN    0x03EF  //   0, 125, 123
#define LCD_MAROON      0x7800  // 123,   0,   0
#define LCD_PURPLE      0x780F  // 123,   0, 123
#define LCD_OLIVE       0x7BE0  // 123, 125,   0
#define LCD_LIGHTGREY   0xC618  // 198, 195, 198
#define LCD_DARKGREY    0x7BEF  // 123, 125, 123
#define LCD_BLUE        0x001F  //   0,   0, 255
#define LCD_GREEN       0x07E0  //   0, 255,   0
#define LCD_CYAN        0x07FF  //   0, 255, 255
#define LCD_RED         0xF800  // 255,   0,   0
#define LCD_MAGENTA     0xF81F  // 255,   0, 255
#define LCD_YELLOW      0xFFE0  // 255, 255,   0
#define LCD_WHITE       0xFFFF  // 255, 255, 255
#define LCD_ORANGE      0xFD20  // 255, 165,   0
#define LCD_GREENYELLOW 0xAFE5  // 173, 255,  41
#define LCD_PINK        0xFC18  // 255, 130, 198


void lcd_init(void);
void lcd_write_byte(uint8_t byte);
void lcd_reg_write8(uint8_t addr, uint8_t data);
void lcd_reg_write16(uint8_t addr, uint16_t data);

#endif
