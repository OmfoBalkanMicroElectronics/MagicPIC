# lib/drivers — Çevre Birimi Sürücüleri

Her sürücü **tek bir klasörde**, tüm arayüz varyantlarıyla birlikte durur. Klasör başına bir
çevre birimi; alt klasör açılmaz.

## Mevcut sürücüler

| Klasör | Cihaz | Durum | Pin | Doküman |
|---|---|---|---|---|
| `lcd0802/` | HD44780 uyumlu 8×2 LCD | ✅ Doğrulandı | 6 veya **3** | [README](lcd0802/README.md) |
| `hc595/` | 74HC595 shift register | ✅ Doğrulandı | 3 | [README](hc595/README.md) |
| `tm1638/` | 7-segment + tuş matrisi | 🔜 Planlandı | 3 | — |

## Sürücü yazım şablonu

Yeni bir sürücü eklerken bu yapıyı izleyin:
drivers/<cihaz>/
├── <cihaz>.h            Public API — uygulamanın gördüğü tek arayüz
├── <cihaz>.c            Protokol/komut katmanı
├── <cihaz>_pins.h       Pin makroları (kullanıcı özelleştirebilir)
├── <cihaz>_if.h         [opsiyonel] Birden fazla bağlantı yolu varsa sözleşme
├── <cihaz>_if_xxx.c     [opsiyonel] Her bağlantı yolu için bir backend
└── README.md            Bağlantı tablosu, API, maliyet, zamanlama

## Zorunlu kurallar

**1) Doğrudan `PORTx`/`TRISx` yazmayın.** `gpio_hal.h` makrolarını kullanın.
LCD_EN_SetHigh();        /  Soyutlanmış /
PORTBbits.RB2 = 1;       /  Kart değişince sürücüyü düzenlemek gerekir /


**2) Pin tanımlarını ayrı header'a alın.** Kullanıcı, sürücü koduna dokunmadan bağlantısını
değiştirebilmeli.

**3) Çağrı derinliğini hesaplayın.** Donanım stack'i **8 seviye**; ISR için 2 seviye pay
bırakın → uygulama zinciri **en fazla 6**. Sık çağrılan alt işlemleri **makro** yapın
(`hc595.h` bunun örneğidir).

**4) `%` ve `/` operatörlerinden kaçının.** XC8 bunlar için kütüphane fonksiyonu çağırır →
gizli **+1 stack seviyesi**. Alternatif:
while (idx >= span) { idx -= span; }    / Modulo yerine, bölme çağrısı yok /


**5) Birden fazla bağlantı yolu varsa backend sözleşmesi kurun.** `lcd_if.h` desenini taklit
edin: protokol katmanı yalnızca 2–3 fonksiyonluk bir sözleşmeye bağlanır, fiziksel yol
`#if` ile seçilir. Kazanç: yeni bağlantı yolu eklemek protokol kodunu **hiç** değiştirmez.

**6) Debug rutini yazın.** `core/debug_test.c` içine `DEBUG_<Cihaz>SelfTest()` ekleyin.
Bu rutin sürücüyü uygulamadan bağımsız doğrulamalı.

**7) `README.md` yazın.** İçinde mutlaka: bağlantı tablosu (pin numaralarıyla), API listesi,
RAM/ROM maliyeti, en derin çağrı zinciri, zamanlama hesabı, bilinen tuzaklar.

## Sürücü bağımsızlığı kontrol listesi

Sürücünüz taşınabilir mi? Şu soruların cevabı "hayır" olmalı:

- [ ] İçinde `PORTx`, `TRISx`, `ANSEL` gibi doğrudan SFR erişimi var mı?
- [ ] Belirli bir pine sabit bağımlılık var mı (`#define` ile soyutlanmamış)?
- [ ] Başka bir sürücünün başlığını dahil ediyor mu?
- [ ] `printf`, `malloc`, `float` kullanıyor mu?
- [ ] Bloklayan sonsuz döngü içeriyor mu?

Son madde özellikle önemli: sürücü **asla** ana döngüyü sahiplenmemeli. Zamanlamayı uygulama
katmanı yönetir (`lcd_scroll` bunun örneğidir).
