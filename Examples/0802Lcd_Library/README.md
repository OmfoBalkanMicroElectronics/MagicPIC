# HD44780 Uyumlu 0802A LCD Sürücü Kütüphanesi

Bu kütüphane, **MagicPIC** projesi kapsamında PIC16F886 mikrodenetleyicisi ile HD44780 (veya uyumlu) kontrolcüsüne sahip 8x2 karakterlik 0802A LCD ekranların yüksek kararlılıkla sürülmesi amacıyla geliştirilmiştir. Sürücü mimarisi, 20 MHz harici osilatör frekansı altında mikro saniye hassasiyetindeki sinyal sürekliliği ve port geçiş kararlılığı dikkate alınarak optimize edilmiştir.

## Öne Çıkan Özellikler

* **4-Bit Veri Yolu Optimizasyonu:** LCD ekran donanımsal olarak 4-bit modunda sürülmektedir. Bu sayede mikrodenetleyicinin pin kaynakları verimli kullanılmaktadır.
* **Low-Voltage Programming (LVP) Uyumluluğu:** `RB3/PGM` pini kütüphane tarafından tamamen izole edilmiştir. LVP modunda çalışan programlayıcı donanımlarla (ZEPPP vb.) dijital I/O çakışması yaşanmaz.
* **Yazılımsal Sinyal Oturma Gecikmeleri:** Yüksek saat hızlarında (20 MHz) PIC port çıkışlarının elektriksel yükselme/düşme sürelerinin (rise/fall time) birbirini ezmesini engelleyen mikro gecikmeler entegre edilmiştir.
* **Yazılımsal Kayan Yazı Motoru:** Donanımsal clipping (veri taşması) sınırlarını aşan uzun metinlerin, ekran boyutuna göre tamponlanarak akıcı bir şekilde kaydırılmasını sağlayan fonksiyon kütüphaneye dahil edilmiştir.

## Donanım Pin Konfigürasyonu

| LCD Sinyali | PIC16F886 Pini | Veri Yönü | Açıklama |
| :--- | :--- | :--- | :--- |
| **RS** | RA0 | Çıkış | Register Seçim Hattı (Komut/Veri) |
| **EN** | RA1 | Çıkış | Tetikleme (Enable) Donanımsal Darbe Hattı |
| **D4** | RB0 | Çıkış | Veri Hattı Bit 0 |
| **D5** | RB1 | Çıkış | Veri Hattı Bit 1 |
| **D6** | RB2 | Çıkış | Veri Hattı Bit 2 |
| **D7** | RB4 | Çıkış | Veri Hattı Bit 3 (`RB3` Programlama pini olarak korunmuştur) |
| **R/W** | *GND* | - | Doğrudan Şasiye Bağlanmalıdır (Sadece Yazma Modu) |

## API Fonksiyonları ve Teknik Tanımları

### `void Lcd_Init(void);`
LCD kontrolcüsünü Hitachi HD44780 standart başlatma protokolüne uygun olarak uyandırır, 4-bit çalışma modunu aktif eder, ekranı açar ve imleci pasif konuma getirir.

### `void Lcd_Clear(void);`
Ekran DDRAM hafızasını tamamen temizler ve imleç adresini başlangıç noktasına (0x00) konumlandırır.

### `void Lcd_Set_Cursor(char row, char col);`
İmleci belirtilen satır (1-2) ve sütun (1-8) koordinatlarına nokta atışı taşır.

### `void Lcd_Write_Char(char a);`
RS hattını lojik-1 seviyesine çekerek ekrana tek bir ASCII karakteri basar.

### `void Lcd_Write_String(const char *str);`
Verilen metin dizisini (`null-terminated string`) karakter karakter LCD ekranına aktarır.

### `void Lcd_Scroll_Text(char row, const char *text, int speed_ms);`
Belirtilen satırda, tanımlanan milisaniye hızında sola doğru akan bir kayan yazı efekti oluşturur. Fonksiyon, metnin ekrana temiz girip çıkabilmesi için başlangıç ve bitiş tamponlarını otomatik yönetir.

## Örnek Kullanım Dokümantasyonu

Kütüphanenin ana projeye dahil edilerek çalıştırılmasına yönelik asgari kod mimarisi şu şekildedir:

```c
#include <xc.h>
#include "lcd0802.h"

#define _XTAL_FREQ 20000000 

#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, MCLRE = ON
#pragma config CP = OFF, CPD = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF
#pragma config LVP = ON, WRT = OFF        

void main(void) {
    OSCCON = 0x00; 
    ANSEL = 0x00; 
    ANSELH = 0x00;
    
    // İlgili port pinleri çıkış olarak konfigüre edilir
    TRISA0 = 0; TRISA1 = 0;
    TRISB0 = 0; TRISB1 = 0; TRISB2 = 0; TRISB4 = 0;

    Lcd_Init(); // Sürücü başlatma prosedürü

    while(1) {
        Lcd_Set_Cursor(2, 1);
        Lcd_Write_String("MagicPIC"); // Alt satır sabit metin
        
        // Üst satırda akıcı kayan yazı simülasyonu
        Lcd_Scroll_Text(1, "        PIC16F886 LCD Kutuphane Testi        ", 300);
    }
}
```

