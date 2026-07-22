/*
 * File:   lcd0802.c
 * Author: BarbaroR4 / OBM
 *
 * Created on 22 July 2026 Wednesday, 22:29
 * MIT License
 * 
 * Copyright (c) 2026 OmfoBalkanMicroElectronics
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ============================================================================
 */




#include "lcd0802.h"

void Lcd_Port(char a) {
    D4 = (a & 0x01) ? 1 : 0; __delay_us(5);
    D5 = (a & 0x02) ? 1 : 0; __delay_us(5);
    D6 = (a & 0x04) ? 1 : 0; __delay_us(5);
    D7 = (a & 0x08) ? 1 : 0; __delay_us(5);
}

void Lcd_Cmd(char a) {
    RS = 0; __delay_us(10);
    Lcd_Port((a >> 4) & 0x0F); __delay_us(10);
    EN = 1; __delay_us(20); EN = 0; __delay_us(20);
    Lcd_Port(a & 0x0F); __delay_us(10);
    EN = 1; __delay_us(20); EN = 0;
    __delay_ms(5);
}

void Lcd_Clear(void) {
    Lcd_Cmd(0x01);
    __delay_ms(5);
}

void Lcd_Set_Cursor(char row, char col) {
    Lcd_Cmd((row == 1 ? 0x80 : 0xC0) + (col - 1));
}

void Lcd_Init(void) {
    RS = 0; EN = 0;
    Lcd_Port(0x00);
    __delay_ms(150);
    
    Lcd_Port(0x03); EN = 1; __delay_us(20); EN = 0; __delay_ms(10);
    Lcd_Port(0x03); EN = 1; __delay_us(20); EN = 0; __delay_ms(2);
    Lcd_Port(0x03); EN = 1; __delay_us(20); EN = 0; __delay_ms(2);
    Lcd_Port(0x02); EN = 1; __delay_us(20); EN = 0; __delay_ms(5);
    
    Lcd_Cmd(0x28);
    Lcd_Cmd(0x0C); // Imlec kapali standart mod
    Lcd_Cmd(0x06);
    Lcd_Clear();
}

void Lcd_Write_Char(char a) {
    RS = 1; __delay_us(10);
    Lcd_Port((a >> 4) & 0x0F); __delay_us(10);
    EN = 1; __delay_us(20); EN = 0; __delay_us(20);
    Lcd_Port(a & 0x0F); __delay_us(10);
    EN = 1; __delay_us(20); EN = 0;
    __delay_us(100);
}

void Lcd_Write_String(const char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        Lcd_Write_Char(str[i]);
    }
}

void Lcd_Scroll_Text(char row, const char *text, int speed_ms) {
    int len = 0;
    while(text[len] != '\0') len++; // Metin uzunlugunu hesapla

    // Ekrana giris ve cikis bosluklari dahil kaydirma motoru
    for(int i = 0; i < (len - 7); i++) {
        Lcd_Set_Cursor(row, 1);
        for(int j = 0; j < 8; j++) {
            Lcd_Write_Char(text[i + j]);
        }
        // Gecikme suresini donguyle esnetiyoruz
        for(int t = 0; t < speed_ms; t += 10) {
            __delay_ms(10);
        }
    }
}
