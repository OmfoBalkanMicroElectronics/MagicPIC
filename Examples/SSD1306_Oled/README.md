# SSD1306 I2C OLED Sürücü ve Grafik Kütüphanesi

Bu kütüphane, **MagicPIC** projesi kapsamında PIC16F886 mikrodenetleyicisi ile I2C protokolü üzerinden haberleşen 0.96" SSD1306 kontrolcülü 128x64 piksel grafik OLED ekranların yüksek kararlılıkla sürülmesi amacıyla geliştirilmiştir. Sürücü mimarisi, donanımsal I2C (MSSP) modülünü Fast Mode (400 kHz) frekansında çalıştırarak metin, dinamik veri ve pürüzsüz geometrik grafikleri asgari hafıza tüketimi ile ekrana aktarır.
![OLED](images/oled_working.gif)

## Öne Çıkan Teknik Özellikler

* **Dinamik I2C Adresleme Desteği:** Proje ihtiyaçlarına göre I2C donanım adresi (`0x3C` veya `0x3D`) fonksiyonlara parametre olarak geçirilerek çoklu ekran veya adres seçimi çalışma zamanında (runtime) yönetilebilir.
* **Hafif Sayı Yazdırma Motoru (`OLED_Write_Int`):** Standart `sprintf` fonksiyonunun yarattığı flash ve RAM şişmelerini engellemek adına, matematiksel basamak bölme yöntemiyle çalışan, negatif/pozitif işaret destekli, performanslı tamsayı dönüşüm algoritması entegre edilmiştir.
* **Yazılımsal Kayan Yazı Motoru:** 128 piksel genişliğindeki ekranda (karakter başına 6 piksel) donanımsal taşmaları (clipping) engelleyen, metni tamponlayarak sola doğru akıtan pencereli kaydırma fonksiyonu içerir.
* **Bresenham Pürüzsüz Çizgi Algoritması:** SSD1306'nın 8 bit dikey sayfa yapısından (`Page Addressing`) kaynaklanan kesik ve kalın geometrik çizim kusurlarını gidermek adına, piksel hassasiyetinde (X: 0-127, Y: 0-63) pürüzsüz çizgi (`Bresenham's Line Algorithm`) ve delta üçgen motoru dahil edilmiştir.

## Donanım Pin Konfigürasyonu

| OLED Sinyali | PIC16F886 Pini | Veri Yönü | Açıklama |
| :--- | :--- | :--- | :--- |
| **VCC** | 5V veya 3.3V | - | Pozitif Besleme Hattı |
| **GND** | GND | - | Ortak Şasi Hattı |
| **SCL** | RC3 (14. Pin) | Giriş / Çıkış | Donanımsal I2C Saat Hattı (MSSP Modülü Kontrolünde) |
| **SDA** | RC4 (15. Pin) | Giriş / Çıkış | Donanımsal I2C Veri Hattı (MSSP Modülü Kontrolünde) |

*Not: I2C hattının elektriksel kararlılığı için SCL-VCC ve SDA-VCC hatları arasına 4.7kΩ değerinde harici Pull-Up dirençlerinin bağlanması zorunludur.*

## API Fonksiyonları ve Teknik Tanımları

### `void OLED_Init(char i2c_addr);`
Donanımsal I2C modülünü 400 kHz hızında yapılandırır, dahili voltaj katlayıcıyı (Charge Pump) aktif eder ve ekranı temizleyerek başlatır.

### `void OLED_Clear(char i2c_addr);`
Ekranın 8 adet veri sayfasını (Page 0 - Page 7) sırayla tarayarak tüm pikselleri lojik-0 seviyesine çeker ve ekranı karartır.

### `void OLED_Set_Cursor(char i2c_addr, char page, char col);`
Yazı modu için imleci dikey sayfa koordinatına (0-7) ve yatay sütun piksel koordinatına (0-127) konumlandırır.

### `void OLED_Write_String(char i2c_addr, const char *str);`
Verilen metin dizisini (`null-terminated string`) dikey 5x7 font sözlüğü üzerinden karakter karakter ekrana basar.

### `void OLED_Write_Int(char i2c_addr, int num);`
İşaretli tamsayıları (integer) RAM tamponu kullanmadan, matematiksel basamak analiziyle doğrudan ASCII karakter serisi olarak ekrana yansıtır.

### `void OLED_Scroll_Text(char i2c_addr, char page, const char *text, int speed_ms);`
Belirtilen dikey sayfada, girilen milisaniye gecikme hızında sola doğru kayan akıcı bir metin bandı oluşturur.

### `void OLED_Draw_Rectangle(char i2c_addr, char start_page, char end_page, char start_col, char end_col);`
Sayfa ve sütun sınırları belirtilen alanda içi boş, kesintisiz bir dikdörtgen/kare geometrisi oluşturur.

### `void OLED_Draw_Triangle(char i2c_addr, char base_y, char top_y, char start_x, char end_x);`
Bresenham çizgi algoritmasını kullanarak piksel koordinat düzeyinde (Y: 0-63, X: 0-127) pürüzsüz ve ince çizgili bir üçgen geometrisi çizer.

---

## Tümleşik Örnek Uygulama Kodları

Kütüphanenin sunduğu tüm özellikleri (Kayan yazı, Hello World metni, dinamik sayaç ve pürüzsüz geometri motoru) tek bir döngüde bir arada çalıştıran örnek `main.c` mimarisi şu şekildedir:

```c
#include <xc.h>
#include "oled_ssd1306.h"

#define _XTAL_FREQ 20000000 

#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, MCLRE = ON
#pragma config CP = OFF, CPD = OFF, BOREN = OFF, IESO = OFF, FCMEN = OFF
#pragma config LVP = ON, WRT = OFF        

#define OLED_ADDR  0x3C

void main(void) {
    OSCCON = 0x00; 
    ANSEL = 0x00; 
    ANSELH = 0x00;
    
    // I2C Donanımsal Pin Yönlendirmeleri
    TRISC3 = 1; 
    TRISC4 = 1; 

    // Ekranı Başlatma Prosedürü
    OLED_Init(OLED_ADDR); 

    int sayac = 0;
    const char kayan_mesaj[] = "MAGICPIC PROJESI - SSD1306 OLED SURUCU KUTUPHANESI TESTI         ";

    // 1. Statik Metin Gösterimi (Hello World Katmanı)
    OLED_Set_Cursor(OLED_ADDR, 2, 4);
    OLED_Write_String(OLED_ADDR, "HELLO WORLD");

    // 2. Geometrik Çizim Gösterimi (Grafik Motoru Katmanı)
    // Sol tarafa içi boş dikdörtgen yerleşimi
    OLED_Draw_Rectangle(OLED_ADDR, 4, 7, 4, 34);
    
    // Sağ tarafa Bresenham algoritması ile pürüzsüz üçgen yerleşimi
    // Parametreler: Taban Y (Piksel), Tepe Y (Piksel), Sol X, Sağ X
    OLED_Draw_Triangle(OLED_ADDR, 60, 35, 45, 85);

    while(1) {
        // 3. Dinamik Veri Gösterimi (Sayaç Katmanı)
        // Dikdörtgenin içine sığacak şekilde koordinatlandırma
        OLED_Set_Cursor(OLED_ADDR, 5, 8);
        OLED_Write_Int(OLED_ADDR, sayac);
        OLED_Write_String(OLED_ADDR, "  "); // Eski hane kalıntılarını temizleme boşluğu

        sayac += 1;
        if(sayac > 99) sayac = 0; // Hane taşmasını önlemek için sınır kontrolü

        // 4. Dinamik Kayan Yazı Gösterimi (Arayüz Metin Bandı Katmanı)
        // En üst satırda (Sayfa 0) metni sola doğru kaydırır
        OLED_Scroll_Text(OLED_ADDR, 0, kayan_mesaj, 100);
        
        // Not: Scroll fonksiyonunun dahili gecikmesi ana döngü hızını otomatik olarak regüle eder.
    }
}
```

