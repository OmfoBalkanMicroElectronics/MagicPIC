# TM1638 LED & KEY Sürücü Kütüphanesi

Bu kütüphane, **MagicPIC** projesi kapsamında PIC16F886 mikrodenetleyicisi ile seri arayüz üzerinden haberleşen TM1638 entegreli LED & KEY modüllerinin kararlı bir şekilde kontrol edilmesi amacıyla geliştirilmiştir. Sürücü mimarisi; 8 adet ortak katot 7-segment göstergeyi, 8 adet bağımsız kırmızı LED'i ve 8 adet dahili butonu asgari pin kaynağı kullanarak (3-wire seri haberleşme) yönetmek üzere optimize edilmiştir.

## Öne Çıkan Özellikler

* **Dinamik Veri Yönü Yönetimi:** Tek bir veri hattı (`TM_DIO`) üzerinden hem veri gönderme (yazma) hem de veri alma (buton okuma) işlemleri, mikrodenetleyicinin TRIS register düzeyinde çalışma zamanlı (runtime) olarak kontrol edilir.
* **Debounce ve Çoklu Basım Koruması:** Buton matris çözümleme algoritması, mekanik buton arklarından kaynaklanan kararsızlıkları ve tek basımda birden fazla veri okunmasını engeller.
* **Donanımsal Adresleme Modları:** Entegrenin otomatik adres artırmalı mod özelliği (`0x40`) aktif kullanılarak, gösterge hafızası tek komut döngüsünde hızlıca güncellenir veya temizlenir.
* **Yazılımsal Font Sözlüğü:** Alfa-nümerik ifadelerin (0-8 arası rakamlar ve E, r, o, P, n, S, F gibi sistem durum karakterleri) 7-segment ekran grubunda net gösterilmesi için optimize edilmiş ortak katot font tablosu içerir.

## Donanım Pin Konfigürasyonu

| Modül Sinyali | PIC16F886 Pini | Veri Yönü | Açıklama |
| :--- | :--- | :--- | :--- |
| **STB** (Strobe) | RA2 | Çıkış | Çip Seçim (Chip Select) Hattı |
| **CLK** (Clock) | RA3 | Çıkış | Seri Saat Sinyali Hattı |
| **DIO** (Data I/O) | RA4 | Giriş / Çıkış | Seri Veri Hattı (*Açık Kollektör yapısı nedeniyle 4.7kΩ Pull-Up direnci önerilir*) |
| **VCC** | 5V | - | Pozitif Besleme Hattı |
| **GND** | GND | - | Ortak Şasi Hattı |

## API Fonksiyonları ve Teknik Tanımları

### `void TM1638_Init(char brightness);`
Sürücü hatlarını başlangıç konumuna getirir, otomatik adresleme modunu aktif eder, tüm ekran hafızasını temizler ve parametre olarak verilen parlaklık seviyesini (0x00 - 0x07 arası) belirler.

### `void TM1638_ClearDisplay(void);`
16 byte'lık entegre ekran RAM alanını sıfırlayarak tüm 7-segment göstergeleri ve harici LED grubunu kapatır.

### `void TM1638_SendCommand(char cmd);`
Donanımsal strobe (`STB`) zamanlaması eşliğinde entegre iç register'larına kontrol ve mod komutları iletir.

### `void TM1638_SetDigit(char digit, char font_idx);`
Belirtilen gösterge hanesine (0-7), kütüphane bünyesindeki font sözlüğünden (`seg_font`) seçilen karakterin segment verisini yazar.

### `void TM1638_SetLED(char led_num, char state);`
Modül üzerindeki ilgili bağımsız LED elemanını lojik durum parametresine (1: Açık, 0: Kapalı) göre kontrol eder.

### `void TM1638_SetLEDs(char mask);`
8 bitlik veri maskesi kullanarak modül üzerindeki tüm LED grubunun durumunu tek bir işlem döngüsünde günceller.

### `char TM1638_ReadButtons(void);`
Entegreye buton okuma komutunu (`0x42`) ileterek dahili tarama matrisini çözer ve basılan butonların lojik haritasını tek bir byte verisi (S1 için bit 0, S8 için bit 7) olarak döndürür.


## Karakter ve Font Sözlüğü (Ortak Katot)

`TM1638_SetDigit(char digit, char font_idx);` fonksiyonunda kullanabileceğiniz, `seg_font[]` dizisinde tanımlı tüm karakterler, hex kodları ve kütüphane indeks numaraları (ID) aşağıdaki tabloda listelenmiştir:

| Karakter Görüntüsü | Hex Verisi (7-Segment) | Kütüphane İndeks Numarası (font_idx) | Kullanım Amacı / Açıklama |
| :---: | :---: | :---: | :--- |
| **0** | `0x3F` | `0` | Nümerik veri gösterimi |
| **1** | `0x06` | `1` | Nümerik veri gösterimi |
| **2** | `0x5B` | `2` | Nümerik veri gösterimi |
| **3** | `0x4F` | `3` | Nümerik veri gösterimi |
| **4** | `0x66` | `4` | Nümerik veri gösterimi |
| **5** | `0x6D` | `5` | Nümerik veri gösterimi |
| **6** | `0x7D` | `6` | Nümerik veri gösterimi |
| **7** | `0x07` | `7` | Nümerik veri gösterimi |
| **8** | `0x7F` | `8` | Nümerik veri gösterimi |
| **E** | `0x79` | `9` | Hata durumu (`Err`) veya Ayar modu (`SEt`) |
| **r** | `0x50` | `10` | Hata durumu mesajı (`Err`) |
| **o** | `0x5C` | `11` | Açık/Onay durumu mesajı (`OPEn`) |
| **P** | `0x73` | `12` | Açık/Onay durumu mesajı (`OPEn`) |
| **n** | `0x37` | `13` | Açık/Onay durumu mesajı (`OPEn`) |
| **S** | `0x6D` | `14` | Ayar modu mesajı (`SEt`) |
| **F** | `0x71` | `15` | Başarısızlık / Arıza mesajı (`FAIL`) |
| *Boşluk* | `0x00` | `16` | Ekranı/Haneyi karartma temizleme işlemi |

*Not: Göstergede küçük 't' harfi efekti oluşturmak için 'r' karakteri (indeks 10) tercih edilmektedir (Örn: `S` [14] + `E` [9] + `r` [10] birleşimi ekranda `SEt` görüntüsü sağlar).*


## Örnek Kullanım Dokümantasyonu

Kütüphanenin temel fonksiyonlarını doğrulamaya yönelik asgari kod mimarisi şu şekildedir:

```c
#include <xc.h>
#include "tm1638.h"

#define _XTAL_FREQ 20000000 

#pragma config FOSC = HS, WDTE = OFF, PWRTE = OFF, MCLRE = ON
#pragma config LVP = ON, WRT = OFF        

void main(void) {
    OSCCON = 0x00; 
    ANSEL = 0x00; 
    ANSELH = 0x00;
    
    // TM1638 STB ve CLK pinleri çıkış yapılır
    TRISA2 = 0; TRISA3 = 0;

    // Kütüphane maksimum parlaklık seviyesi ile başlatılır
    TM1638_Init(TM_BRIGHTNESS_MAX); 

    while(1) {
        // Göstergenin ilk 4 hanesine sırasıyla '1', '2', '3', '4' basar
        TM1638_SetDigit(0, 1);
        TM1638_SetDigit(1, 2);
        TM1638_SetDigit(2, 3);
        TM1638_SetDigit(3, 4);

        char buton_durum = TM1638_ReadButtons();
        
        // Eğer S1 butonuna basılırsa ilk LED'i yakar, basılmazsa söndürür
        if(buton_durum & 0x01) {
            TM1638_SetLED(0, 1);
        } else {
            TM1638_SetLED(0, 0);
        }
    }
}
```
