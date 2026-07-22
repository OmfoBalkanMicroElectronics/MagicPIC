# PIC16F886 TM1638 ve HD44780 LCD Entegrasyon Uygulaması

Bu çalışma, MagicPIC projesi kapsamında PIC16F886 mikrodenetleyicisinin 20 MHz harici osilatör frekansı altında iki farklı harici çevre birimiyle (HD44780 uyumlu 0802A LCD ve TM1638 LED & KEY sürücüsü) es zamanlı ve senkronize çalışmasını sağlamak amacıyla geliştirilmiştir. Sistem sigorta ayarlarında Low-Voltage Programming (LVP) modu aktif bırakılarak RB3/PGM pini programlama donanımı için izole edilmiş, dijital I/O çakışmaları engellenmiştir.

## Sistem Mimarisi ve Fonksiyonel Senaryo

* **Çift Ekran Kontrolü ve Veri Hizalama:** Kullanıcı arayüzünün ilk katmanında 0802A LCD üst satırı durum mesajlarına ayrılmıştır. Alt satırda ise girilen şifre karakterleri maskelenerek (********) listelenir. Boyutsal sınırlamalar nedeniyle oluşabilecek veri taşmaları (clipping) yazılımsal satır yönetimi ile engellenmiştir. TM1638 entegresi ise basılan nümerik değerleri anlık olarak kendi 7-segment ekran grubu üzerinde görüntüler.
* **Görsel ve Donanımsal Geri Bildirim:** Yetkilendirme doğrulaması başarılı olduğunda RC0 pinine bağlı kapı kilidi simülasyon rölesi lojik-1 seviyesine çekilir ve TM1638 üzerindeki LED grubu es zamanlı olarak periyodik kesmelerle flasör moduna geçer. Hatalı girişlerde ise LCD üzerinde hata uyarısı verilirken, LED grubu sağa ve sola kayan bir tarama algoritması çalıştırmaktadır.
* **Çalışma Zamanlı Şifre Belirleme (Dinamik Kayıt):** Sistem belleğinde başlangıç değeri olarak 1-2-3-4-5-6-7-8 dizisi tutulmaktadır. Sistem bekleme modundayken S8 butonuna donanımsal debounce süresinin üzerinde (2 saniye) basılı tutulduğunda, sistem kesme simülasyonuna girerek yeni şifre belirleme modunu aktif eder. Butonlar üzerinden alınan yeni 8 haneli dizi, çalışma zamanlı olarak sistem RAM belleğine işlenir ve güncel yetkilendirme anahtarı haline gelir.

## Donanım Pin Konfigürasyon Tablosu

| Çevre Birimi | Sinyal Adı | PIC16F886 Pini | Veri Yönü | Açıklama |
| :--- | :--- | :--- | :--- | :--- |
| **HD44780 LCD** | RS | RA0 | Çıkış | Register Seçim Hattı |
| **HD44780 LCD** | EN | RA1 | Çıkış | Tetikleme (Enable) Hattı |
| **HD44780 LCD** | D4 - D7 | RB0, RB1, RB2, RB4 | Çıkış | 4-Bit Veri Yolu (RB3 Muaf) |
| **TM1638 Modülü** | STB | RA2 | Çıkış | Strobe (Chip Select) Hattı |
| **TM1638 Modülü** | CLK | RA3 | Çıkış | Seri Saat (Clock) Hattı |
| **TM1638 Modülü** | DIO | RA4 | Giriş / Çıkış | Seri Veri Hattı (Açık Kollektör) |
| **Aktüatör** | RELAY | RC0 | Çıkış | Kilit Simülasyon Çıkışı |

**Not**: Bu proje için gereken kütüphaneler,"Examples" klasöründe bulunur. Kod hakkında ise kodu şuan tenizliyorum,temiz hale gelince yükleyeceğim(en kısa zamanda)
