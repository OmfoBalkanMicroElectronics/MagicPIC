# MagicPIC LCD0802 Driver

Bu sürücü, HD44780 uyumlu 8x2 karakter LCD modüllerini 4-bit modda sürmek için hazırlanmıştır.

## Özellikler

- 4-bit veri yolu desteği
- Configurable pin yapısı
- Ortak `mp_gpio` katmanı ile çalışma
- Temel karakter yazdırma
- İmleç konumlandırma
- Ekran temizleme
- Kayan yazı (scroll text)

## Gerekli Dosyalar

- `system_config.h`
- `mp_gpio.h`
- `mp_gpio.c`
- `lcd0802.h`
- `lcd0802.c`

## Bağımlılıklar

Bu sürücü aşağıdaki ortak bileşenlere ihtiyaç duyar:

- `system_config.h` → `_XTAL_FREQ` tanımı için
- `mp_gpio.h / mp_gpio.c` → pin soyutlama katmanı için

## Donanım Arayüzü

Sürücü aşağıdaki LCD pinlerini kullanır:

- `RS`
- `EN`
- `D4`
- `D5`
- `D6`
- `D7`

`RW` pini bu sürücüde kullanılmaz, doğrudan **GND**'ye bağlanmalıdır.

## Test Edilen Pin Eşleşmesi

Aşağıdaki tablo, test edilen örnek bağlantıdır:

| LCD Pin | PIC16F886 Pin |
|---|---|
| RS | RA0 |
| EN | RA1 |
| D4 | RB0 |
| D5 | RB1 |
| D6 | RB2 |
| D7 | RB4 |
| RW | GND |

## LCD Besleme Bağlantıları

| LCD Pin | Bağlantı |
|---|---|
| VSS | GND |
| VDD | +5V |
| VO / VEE | Kontrast potansiyometresi orta ucu |
| A / LED+ | +5V (modüle göre dirençli veya doğrudan) |
| K / LED- | GND |

## Önemli Notlar

- PIC16F886 üzerinde `RA0` ve `RA1` gibi pinler analog gelebilir.
- Bu nedenle uygulama başlangıcında aşağıdaki ayarlar yapılmalıdır:
```
ANSEL = 0x00;
ANSELH = 0x00;
```


- `LVP = ON` kullanılıyorsa `RB3` pininden kaçınılması tavsiye edilir.
- Bu örnekte `RB3` kullanılmamıştır.

## Kullanılan Yapı
```
typedef struct
{
mp_gpio_pin_t rs;
mp_gpio_pin_t en;
mp_gpio_pin_t d4;
mp_gpio_pin_t d5;
mp_gpio_pin_t d6;
mp_gpio_pin_t d7;
uint8_t cols;
uint8_t rows;
} lcd0802_t;
```


## Kullanılabilir Fonksiyonlar

- `LCD0802_Init()`
- `LCD0802_Clear()`
- `LCD0802_Home()`
- `LCD0802_SetCursor()`
- `LCD0802_WriteChar()`
- `LCD0802_WriteString()`
- `LCD0802_WriteStringAt()`
- `LCD0802_ScrollText()`

## Örnek Pin Atama
```
static lcd0802_t lcd;
void LCD_AssignPins(void)
{
lcd.rs.port        = &PORTA;
lcd.rs.tris        = &TRISA;
lcd.rs.analog_reg  = &ANSEL;
lcd.rs.port_mask   = (1u << 0);
lcd.rs.analog_mask = (1u << 0);
lcd.en.port        = &PORTA;
lcd.en.tris        = &TRISA;
lcd.en.analog_reg  = &ANSEL;
lcd.en.port_mask   = (1u << 1);
lcd.en.analog_mask = (1u << 1);
lcd.d4.port        = &PORTB;
lcd.d4.tris        = &TRISB;
lcd.d4.analog_reg  = 0;
lcd.d4.port_mask   = (1u << 0);
lcd.d4.analog_mask = 0;
lcd.d5.port        = &PORTB;
lcd.d5.tris        = &TRISB;
lcd.d5.analog_reg  = 0;
lcd.d5.port_mask   = (1u << 1);
lcd.d5.analog_mask = 0;
lcd.d6.port        = &PORTB;
lcd.d6.tris        = &TRISB;
lcd.d6.analog_reg  = 0;
lcd.d6.port_mask   = (1u << 2);
lcd.d6.analog_mask = 0;
lcd.d7.port        = &PORTB;
lcd.d7.tris        = &TRISB;
lcd.d7.analog_reg  = 0;
lcd.d7.port_mask   = (1u << 4);
lcd.d7.analog_mask = 0;
lcd.cols = 8;
lcd.rows = 2;
}
```


## Basit Kullanım Örneği
```
#include <xc.h>
#include <stdint.h>
#include "system_config.h"
#include "mp_gpio.h"
#include "lcd0802.h"
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config MCLRE = ON
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config LVP = ON
static lcd0802_t lcd;
void LCD_AssignPins(void)
{
lcd.rs.port        = &PORTA;
lcd.rs.tris        = &TRISA;
lcd.rs.analog_reg  = &ANSEL;
lcd.rs.port_mask   = (1u << 0);
lcd.rs.analog_mask = (1u << 0);
lcd.en.port        = &PORTA;
lcd.en.tris        = &TRISA;
lcd.en.analog_reg  = &ANSEL;
lcd.en.port_mask   = (1u << 1);
lcd.en.analog_mask = (1u << 1);
lcd.d4.port        = &PORTB;
lcd.d4.tris        = &TRISB;
lcd.d4.analog_reg  = 0;
lcd.d4.port_mask   = (1u << 0);
lcd.d4.analog_mask = 0;
lcd.d5.port        = &PORTB;
lcd.d5.tris        = &TRISB;
lcd.d5.analog_reg  = 0;
lcd.d5.port_mask   = (1u << 1);
lcd.d5.analog_mask = 0;
lcd.d6.port        = &PORTB;
lcd.d6.tris        = &TRISB;
lcd.d6.analog_reg  = 0;
lcd.d6.port_mask   = (1u << 2);
lcd.d6.analog_mask = 0;
lcd.d7.port        = &PORTB;
lcd.d7.tris        = &TRISB;
lcd.d7.analog_reg  = 0;
lcd.d7.port_mask   = (1u << 4);
lcd.d7.analog_mask = 0;
lcd.cols = 8;
lcd.rows = 2;
}
void main(void)
{
ANSEL = 0x00;
ANSELH = 0x00;
LCD_AssignPins();
LCD0802_Init(&lcd);
LCD0802_SetCursor(&lcd, 0, 0);
LCD0802_WriteString(&lcd, "BOOT");
LCD0802_SetCursor(&lcd, 1, 0);
LCD0802_WriteString(&lcd, "OK");
__delay_ms(1500);
LCD0802_Clear(&lcd);
LCD0802_ScrollText(&lcd, 0, "MagicPIC LCD Driver Test", 200);
while (1)
{
}
}

```


## Sorun Giderme

### Hiç görüntü yoksa
- VDD / VSS bağlantısını kontrol edin
- kontrast potansiyometresini ayarlayın
- `ANSEL` ve `ANSELH` değerlerini kontrol edin
- `RW` pininin GND'ye bağlı olduğundan emin olun

### Anlamsız karakterler çıkıyorsa
- `D4-D7` bağlantı sırasını kontrol edin
- `_XTAL_FREQ` tanımını kontrol edin
- clock kaynağının gerçekten 20 MHz olduğundan emin olun

### Scroll düzgün çalışmıyorsa
- `cols` değerinin LCD ile uyumlu olduğunu kontrol edin
- çok uzun gecikme değerleri kullanmayın
- bence çok pro bir kütüphane :)
