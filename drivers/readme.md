Burada kütüphaneler/sürücüler olacaktır.

# MagicPIC Common Dependencies

Bu projedeki bazı sürücüler, doğrudan sabit pin makroları kullanmak yerine ortak bir GPIO soyutlama katmanı üzerinden çalışır.

## Gerekli Dosyalar

Aşağıdaki dosyalar ilgili sürücülerin bağımlılığıdır:

- `system_config.h`
- `mp_gpio.h`
- `mp_gpio.c`

## system_config.h

Bu dosya sistem saat frekansını merkezi olarak tanımlar.

Örnek:
'''
#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H
#define _XTAL_FREQ 20000000UL
#endif
'''


## mp_gpio katmanı

`mp_gpio` katmanı, pinlerin:

- yön ayarı
- lojik seviye sürülmesi
- okunması

işlemlerini ortak bir yapı ile yönetir.

Bu sayede:

- sürücüler board'a sabitlenmez
- pinler uygulama tarafından atanabilir
- aynı sürücü farklı kartlarda tekrar kullanılabilir

## Kullanılan Veri Yapısı
'''
typedef struct
{
volatile unsigned char *port;
volatile unsigned char *tris;
volatile unsigned char *analog_reg;
uint8_t port_mask;
uint8_t analog_mask;
} mp_gpio_pin_t;
'''


## Tipik Kullanım

Bir pin tanımı örneği:
'''
lcd.rs.port        = &PORTA;
lcd.rs.tris        = &TRISA;
lcd.rs.analog_reg  = &ANSEL;
lcd.rs.port_mask   = (1u << 0);
lcd.rs.analog_mask = (1u << 0);
'''


## Notlar

- PIC16F886 üzerinde bazı pinler analog varsayılan gelebilir.
- Bu nedenle `ANSEL` ve `ANSELH` başlangıçta doğru ayarlanmalıdır.
- `_XTAL_FREQ` her sürücü için `system_config.h` üzerinden sağlanmalıdır.
