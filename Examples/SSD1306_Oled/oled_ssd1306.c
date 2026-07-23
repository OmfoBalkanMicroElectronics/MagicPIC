/* 
 * File:   oled_ssd1306.h
 * Author: Barbaror4 / OBM
 * Comments: MagicPIC SSD1306 I2C OLED Sürücü Kütüphanesi
 * Revision history: 1.0
 */


#include "oled_ssd1306.h"

// 5x7 Minimal Karakter Font Sözlü?ü (ASCII 32 - 90 aras? temel karakterler)
const char oled_font[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space (32)
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0 (48)
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x3E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A (65)
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}  // Z (90)
};

// ==========================================
// DONANIMSAI I2C (MSSP) SÜRÜCÜSÜ
// ==========================================
void I2C_Init(void) {
    TRISC3 = 1; // SCL Giri? moduna al?n?r
    TRISC4 = 1; // SDA Giri? moduna al?n?r
    
    SSPSTAT |= 0x80; // Slew rate kontrolü kapat?ld? (100kHz / 400kHz standart)
    SSPCON = 0x28;   // I2C Master modu aktif edildi, SSP aktif [0x28]
    SSPADD = (_XTAL_FREQ / (4 * I2C_CLOCK_SPEED)) - 1; // Baud rate hesab?
}

void I2C_Start(void) {
    SEN = 1; while(SEN); // Start ko?ulu üret ve tamamlanmas?n? bekle
}

void I2C_Stop(void) {
    PEN = 1; while(PEN); // Stop ko?ulu üret ve tamamlanmas?n? bekle
}

void I2C_Write(char data) {
    SSPBUF = data; while(BF); // Veriyi tampona yaz ve gönderim bitene kadar bekle
    char ack = ACKSTAT; // ACK durumunu temizlemek için oku
}

// ==========================================
// SSD1306 OLED PROTOKOLÜ
// ==========================================
void OLED_Write_Cmd(char i2c_addr, char cmd) {
    I2C_Start();
    I2C_Write(i2c_addr << 1); // Cihaz adresi (Yazma biti eklenerek kayd?r?l?r)
    I2C_Write(0x00);          // Control Byte: Sadece bir sonraki byte komuttur
    I2C_Write(cmd);
    I2C_Stop();
}

void OLED_Write_Data(char i2c_addr, char data) {
    I2C_Start();
    I2C_Write(i2c_addr << 1);
    I2C_Write(0x40);          // Control Byte: Veri ak??? (Data stream) modu
    I2C_Write(data);
    I2C_Stop();
}

void OLED_Init(char i2c_addr) {
    I2C_Init(); // I2C hatt?n? haz?rla
    __delay_ms(100); // Ekran voltaj oturma süresi
    
    OLED_Write_Cmd(i2c_addr, SSD1306_DISPLAYOFF);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETDISPLAYCLOCKDIV); OLED_Write_Cmd(i2c_addr, 0x80);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETMULTIPLEX);        OLED_Write_Cmd(i2c_addr, 0x3F);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETDISPLAYOFFSET);   OLED_Write_Cmd(i2c_addr, 0x00);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETSTARTLINE | 0x00);
    OLED_Write_Cmd(i2c_addr, SSD1306_CHARGEPUMP);         OLED_Write_Cmd(i2c_addr, 0x14); // Dahili voltaj katlay?c? aç?k
    OLED_Write_Cmd(i2c_addr, SSD1306_MEMORYMODE);         OLED_Write_Cmd(i2c_addr, 0x02); // Page Addressing Mode (Sayfa Adresleme)
    OLED_Write_Cmd(i2c_addr, SSD1306_SEGREMAP);
    OLED_Write_Cmd(i2c_addr, SSD1306_COMSCANDEC);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETCOMPINS);         OLED_Write_Cmd(i2c_addr, 0x12);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETCONTRAST);        OLED_Write_Cmd(i2c_addr, 0xCF);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETPRECHARGE);       OLED_Write_Cmd(i2c_addr, 0xF1);
    OLED_Write_Cmd(i2c_addr, SSD1306_SETVCOMDETECT);      OLED_Write_Cmd(i2c_addr, 0x40);
    OLED_Write_Cmd(i2c_addr, SSD1306_DISPLAYALLON_RESUME);
    OLED_Write_Cmd(i2c_addr, SSD1306_NORMALDISPLAY);
    OLED_Write_Cmd(i2c_addr, SSD1306_DISPLAYON);
    
    OLED_Clear(i2c_addr); // Ekran? temiz ba?lat
}

void OLED_Clear(char i2c_addr) {
    // 8 sayfan?n (Page 0 - Page 7) her birindeki 128 sütunu s?f?rla
    for(char page = 0; page < 8; page++) {
        OLED_Set_Cursor(i2c_addr, page, 0);
        I2C_Start();
        I2C_Write(i2c_addr << 1);
        I2C_Write(0x40);
        for(char col = 0; col < 128; col++) {
            I2C_Write(0x00);
        }
        I2C_Stop();
    }
}

void OLED_Set_Cursor(char i2c_addr, char page, char col) {
    if(page > 7 || col > 127) return;
    OLED_Write_Cmd(i2c_addr, 0xB0 + page);             // Sayfa adresi ayar?
    OLED_Write_Cmd(i2c_addr, 0x00 + (col & 0x0F));      // Sütun alt adresi ayar?
    OLED_Write_Cmd(i2c_addr, 0x10 + ((col >> 4) & 0x0F)); // Sütun üst adresi ayar?
}

void OLED_Write_Char(char i2c_addr, char c) {
    if(c < 32 || c > 90) c = 32; // Font s?n?r kontrolü (Sadece büyük harf ve temel karakterler)
    
    int idx = c - 32;
    I2C_Start();
    I2C_Write(i2c_addr << 1);
    I2C_Write(0x40);
    for(char i = 0; i < 5; i++) {
        I2C_Write(oled_font[idx][i]); // Karakter dikey kolon verilerini gönder
    }
    I2C_Write(0x00); // Karakterler aras? 1 piksel bo?luk
    I2C_Stop();
}

void OLED_Write_String(char i2c_addr, const char *str) {
    for(int i = 0; str[i] != '\0'; i++) {
        OLED_Write_Char(i2c_addr, str[i]);
    }
} // <-- Bu parantezin kapal? oldu?unu kesinlikle kontrol edin.

void OLED_Write_Int(char i2c_addr, int num) {
    char buf[7]; // Maksimum hane boyutu (6 karakter + null) aç?kça tan?mland?
    int i = 0;
    
    if (num == 0) {
        OLED_Write_Char(i2c_addr, '0');
        return;
    }
    
    if (num < 0) {
        OLED_Write_Char(i2c_addr, '-');
        num = -num;
    }
    
    while (num > 0) {
        buf[i++] = (char)((num % 10) + '0'); // char dönü?ümü eklendi
        num /= 10;
    }
    
    while (i > 0) {
        OLED_Write_Char(i2c_addr, buf[--i]);
    }
}
void OLED_Scroll_Text(char i2c_addr, char page, const char *text, int speed_ms) {
    int len = 0;
    while(text[len] != '\0') len++; // Metin uzunlu?unu hesapla

    // E?er metin ekrana s??acak kadar k?saysa kayd?rmaya gerek yok, direkt bas
    if (len <= 21) {
        OLED_Set_Cursor(i2c_addr, page, 0);
        OLED_Write_String(i2c_addr, text);
        return;
    }

    // Karakter kayd?rma penceresi döngüsü
    for(int i = 0; i < (len - 20); i++) {
        OLED_Set_Cursor(i2c_addr, page, 0); // Her ad?mda imleci ilgili sat?r?n ba??na çek
        
        // Yan yana maksimum 21 karakter s??d?r?yoruz
        for(int j = 0; j < 21; j++) {
            OLED_Write_Char(i2c_addr, text[i + j]);
        }
        
        // Hassas gecikme döngüsü
        for(int t = 0; t < speed_ms; t += 10) {
            __delay_ms(10);
        }
    }
}
// Yatay Çizgi Çizme Fonksiyonu (Seçilen sayfada belirtilen uzunlukta düz çizgi çizer)
void OLED_Draw_H_Line(char i2c_addr, char page, char start_col, char length) {
    if(page > 7 || start_col > 127) return;
    OLED_Set_Cursor(i2c_addr, page, start_col);
    I2C_Start();
    I2C_Write((char)(i2c_addr << 1));
    I2C_Write(0x40);
    for(char i = 0; i < length; i++) {
        if((start_col + i) > 127) break;
        I2C_Write(0x08); // Sayfan?n tam ortas?ndan yatay bir çizgi pikseli (0x08: 00001000)
    }
    I2C_Stop();
}

// ?çi Bo? Dikdörtgen/Kare Çizme Fonksiyonu
void OLED_Draw_Rectangle(char i2c_addr, char start_page, char end_page, char start_col, char end_col) {
    // Üst ve Alt yatay çizgileri çiz
    OLED_Set_Cursor(i2c_addr, start_page, start_col);
    I2C_Start(); I2C_Write((char)(i2c_addr << 1)); I2C_Write(0x40);
    for(char c = start_col; c <= end_col; c++) I2C_Write(0x01); // Üst çizgi pikseli
    I2C_Stop();

    OLED_Set_Cursor(i2c_addr, end_page, start_col);
    I2C_Start(); I2C_Write((char)(i2c_addr << 1)); I2C_Write(0x40);
    for(char c = start_col; c <= end_col; c++) I2C_Write(0x80); // Alt çizgi pikseli
    I2C_Stop();

    // Sol ve Sa? dikey duvarlar? ör
    for(char p = start_page; p <= end_page; p++) {
        OLED_Set_Cursor(i2c_addr, p, start_col);
        OLED_Write_Data(i2c_addr, 0xFF); // Sol sütun tam dolu piksel
        OLED_Set_Cursor(i2c_addr, p, end_col);
        OLED_Write_Data(i2c_addr, 0xFF); // Sa? sütun tam dolu piksel
    }
}

// Nokta Çizme Fonksiyonu (Piksel hassasiyetinde koordinat belirler)
void OLED_Draw_Pixel(char i2c_addr, char page, char col, char bit_pos) {
    if(page > 7 || col > 127 || bit_pos > 7) return;
    OLED_Set_Cursor(i2c_addr, page, col);
    OLED_Write_Data(i2c_addr, (char)(1 << bit_pos));
}

// Pürüzsüz Çizgi Çizme Algoritmas? (Bresenham's Line Algorithm)
void OLED_Draw_Line_Smooth(char i2c_addr, int x0, int y0, int x1, int y1) {
    int dx = (x1 > x0) ? (x1 - x0) : (x0 - x1);
    int dy = (y1 > y0) ? (y1 - y0) : (y0 - y1);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        // x ve y koordinatlar?n? SSD1306'n?n Sayfa ve Bit pozisyonuna bölüyoruz
        char current_page = (char)(y0 / 8);
        char current_bit  = (char)(y0 % 8);
        
        // Sadece ilgili pikseli ekrana bas?yoruz, sütunu doldurmuyoruz
        OLED_Draw_Pixel(i2c_addr, current_page, (char)x0, current_bit);

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 <  dx) { err += dx; y0 += sy; }
    }
}

// Yeni ve Pürüzsüz ÜCGEN Çizme Fonksiyonu
void OLED_Draw_Triangle(char i2c_addr, char base_y, char top_y, char start_x, char end_x) {
    // Tepe noktas?n?n X koordinat?n? tam orta nokta olarak hesapla
    char mid_x = start_x + ((end_x - start_x) / 2);
    
    // 1. Sol kenar? çiz (Tepe noktas?ndan sol tabana pürüzsüz çizgi)
    OLED_Draw_Line_Smooth(i2c_addr, mid_x, top_y, start_x, base_y);
    
    // 2. Sa? kenar? çiz (Tepe noktas?ndan sa? tabana pürüzsüz çizgi)
    OLED_Draw_Line_Smooth(i2c_addr, mid_x, top_y, end_x, base_y);
    
    // 3. Taban çizgisini çiz (Sol tabandan sa? tabana düz çizgi)
    OLED_Draw_Line_Smooth(i2c_addr, start_x, base_y, end_x, base_y);
}

