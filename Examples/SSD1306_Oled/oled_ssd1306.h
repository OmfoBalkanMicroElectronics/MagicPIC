/* 
 * File:   oled_ssd1306.h
 * Author: Barbaror4 / OBM
 * Comments: MagicPIC SSD1306 I2C OLED Sürücü Kütüphanesi
 * Revision history: 1.0
 */

#ifndef OLED_SSD1306_H
#define	OLED_SSD1306_H

#include <xc.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

// I2C Donan?msal H?z Tan?m? (400 kHz Fast Mode)
#define I2C_CLOCK_SPEED  400000

// SSD1306 Temel Komut Makrolar?
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETMULTIPLEX         0xA8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_SEGREMAP            0xA1
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETCOMPINS          0x12
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_DISPLAYON           0xAF

// Temel I2C ve OLED Fonksiyon Prototipleri
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(char data);

void OLED_Write_Cmd(char i2c_addr, char cmd);
void OLED_Write_Data(char i2c_addr, char data);
void OLED_Init(char i2c_addr);
void OLED_Clear(char i2c_addr);
void OLED_Set_Cursor(char i2c_addr, char page, char col);
void OLED_Write_Char(char i2c_addr, char c);
void OLED_Write_String(char i2c_addr, const char *str);
void OLED_Write_Int(char i2c_addr, int num);
void OLED_Scroll_Text(char i2c_addr, char page, const char *text, int speed_ms);
void OLED_Draw_H_Line(char i2c_addr, char page, char start_col, char length);
void OLED_Draw_Rectangle(char i2c_addr, char start_page, char end_page, char start_col, char end_col);
void OLED_Draw_Triangle(char i2c_addr, char base_page, char top_page, char start_col, char end_col);


#endif	/* OLED_SSD1306_H */
