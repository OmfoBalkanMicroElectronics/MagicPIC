# MagicPIC TM1638 Driver

Bu sürücü, TM1638 tabanlı 8 digit 7-segment + 8 LED + 8 button modülleri PIC16F886 üzerinde sürmek için hazırlanmıştır.

## Özellikler

- Configurable pin yapısı
- Ortak `mp_gpio` katmanı ile çalışma
- 7-segment karakter gösterimi
- Sayı gösterimi
- Hex gösterimi
- LED kontrolü
- Buton okuma
- Shadow RAM tabanlı daha stabil ekran/LED güncelleme

## Gerekli Dosyalar

- `system_config.h`
- `mp_gpio.h`
- `mp_gpio.c`
- `tm1638.h`
- `tm1638.c`

## Bağımlılıklar

Bu sürücü aşağıdaki ortak bileşenlere ihtiyaç duyar:

- `system_config.h`
- `mp_gpio.h`
- `mp_gpio.c`

## Donanım Arayüzü

TM1638 modülü üç hat üzerinden sürülür:

- `STB`
- `CLK`
- `DIO`

## Test Edilen Pin Eşleşmesi

Yeni test edilen ve stabil çalışan bağlantı:

| TM1638 Pin | PIC16F886 Pin |
|---|---|
| STB | RB0 |
| CLK | RB1 |
| DIO | RB2 |

## Önemli Notlar

- `RB3` bu örnekte kullanılmaz.
- `LVP = ON` kullanıldığında `RB3` pininden kaçınmak önerilir.
- TM1638 için önceki denemelerde `RA4` kullanılmış olabilir; ancak nihai testlerde `RB0/RB1/RB2` yapısı tercih edilmiştir.

## Kullanılan Yapı
```
typedef struct
{
mp_gpio_pin_t stb;
mp_gpio_pin_t clk;
mp_gpio_pin_t dio;
uint8_t brightness;
uint8_t ram[16];
} tm1638_t;

```


## Kullanılabilir Fonksiyonlar

- `TM1638_Init()`
- `TM1638_Refresh()`
- `TM1638_ClearDisplay()`
- `TM1638_ClearLEDs()`
- `TM1638_AllLEDsOn()`
- `TM1638_SetBrightness()`
- `TM1638_SetDigitRaw()`
- `TM1638_SetCharAt()`
- `TM1638_SetText()`
- `TM1638_SetNumber()`
- `TM1638_SetNumberZeroPad()`
- `TM1638_SetHex()`
- `TM1638_SetLED()`
- `TM1638_SetLEDs()`
- `TM1638_ReadButtons()`

## Örnek Pin Atama
```
static tm1638_t tm;
void TM1638_AssignPins(void)
{
tm.stb.port        = &PORTB;
tm.stb.tris        = &TRISB;
tm.stb.analog_reg  = &ANSELH;
tm.stb.port_mask   = (1u << 0);
tm.stb.analog_mask = (1u << 0);
tm.clk.port        = &PORTB;
tm.clk.tris        = &TRISB;
tm.clk.analog_reg  = &ANSELH;
tm.clk.port_mask   = (1u << 1);
tm.clk.analog_mask = (1u << 1);
tm.dio.port        = &PORTB;
tm.dio.tris        = &TRISB;
tm.dio.analog_reg  = &ANSELH;
tm.dio.port_mask   = (1u << 2);
tm.dio.analog_mask = (1u << 2);
tm.brightness = TM1638_BRIGHTNESS_MED;


```


## Basit Kullanım Örneği
```

#include <xc.h>
#include <stdint.h>
#include "system_config.h"
#include "mp_gpio.h"
#include "tm1638.h"
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
static tm1638_t tm;
void TM1638_AssignPins(void)
{
tm.stb.port        = &PORTB;
tm.stb.tris        = &TRISB;
tm.stb.analog_reg  = &ANSELH;
tm.stb.port_mask   = (1u << 0);
tm.stb.analog_mask = (1u << 0);
tm.clk.port        = &PORTB;
tm.clk.tris        = &TRISB;
tm.clk.analog_reg  = &ANSELH;
tm.clk.port_mask   = (1u << 1);
tm.clk.analog_mask = (1u << 1);
tm.dio.port        = &PORTB;
tm.dio.tris        = &TRISB;
tm.dio.analog_reg  = &ANSELH;
tm.dio.port_mask   = (1u << 2);
tm.dio.analog_mask = (1u << 2);
tm.brightness = TM1638_BRIGHTNESS_MED;
}
void main(void)
{
uint8_t i;
ANSEL = 0x00;
ANSELH = 0x00;
TM1638_AssignPins();
TM1638_Init(&tm, TM1638_BRIGHTNESS_MED);
TM1638_SetText(&tm, "12345678");
__delay_ms(1200);
TM1638_SetText(&tm, "LEDS    ");
__delay_ms(500);
for (i = 0u; i < 8u; i++)
{
TM1638_SetLEDs(&tm, (uint8_t)(1u << i));
__delay_ms(250);
}
TM1638_AllLEDsOn(&tm);
__delay_ms(800);
TM1638_ClearLEDs(&tm);
__delay_ms(800);
TM1638_SetText(&tm, "BEEF    ");
__delay_ms(1000);
TM1638_SetText(&tm, "COUNT   ");
__delay_ms(700);
for (i = 0u; i < 100u; i++)
{
TM1638_SetNumberZeroPad(&tm, i);
__delay_ms(120);
}
TM1638_SetText(&tm, "READY   ");
while (1)
{
}
}

```

## Buton Testi Örneği

```

void main(void)
{
uint8_t buttons;
ANSEL = 0x00;
ANSELH = 0x00;
TM1638_AssignPins();
TM1638_Init(&tm, TM1638_BRIGHTNESS_MED);
TM1638_SetText(&tm, "BUTTONS ");
while (1)
{
buttons = TM1638_ReadButtons(&tm);
TM1638_SetLEDs(&tm, buttons);
__delay_ms(50);
}
}

```



## Karakter Desteği Hakkında

TM1638 bir 7-segment göstergedir. Bu nedenle tüm harfler aynı kalitede gösterilemez.

En iyi görünen örnekler:
- `BEEF`
- `SAFE`
- `HELLO`
- `COUNT`
- `READY`

Bazı harfler yaklaşık gösterilir:
- `K`
- `M`
- `N`
- `Q`
- `W`
- `X`

Bu sınırlama yazılımdan çok 7-segment donanım yapısından kaynaklanır.

## Sorun Giderme

### Yazılar düzgün ama LED'ler çalışmıyorsa
- LED modülünü ayrı test edin
- tek tek LED yürütme testi yapın
- fiziksel LED arızası ihtimalini multimetre ile kontrol edin

### Butonlar çalışmıyorsa
- `DIO` hattının doğru pine bağlı olduğundan emin olun
- `DIO` hattının input/output yön değiştirme mantığını bozacak ek devre olmadığını kontrol edin

### Segmentler karışık görünüyorsa
- `CLK`, `STB`, `DIO` pin sırasını kontrol edin
- `_XTAL_FREQ` ve gerçek osilatör uyumunu kontrol edin
