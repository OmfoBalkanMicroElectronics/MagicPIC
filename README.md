## MagicPIC


MagicPIC, klasik Microchip PIC mikrodenetleyicilerini daha yakından tanımak, belgelemek ve geliştirmek amacıyla oluşturulmuş açık kaynaklı bir gömülü sistem projesidir.


Proje, **PIC16F886** mikrodenetleyicisini temel almakta olup; donanım tasarımı, çevre birimi sürücüleri, örnek uygulamalar ve kapsamlı dokümantasyon geliştirmeyi hedeflemektedir.


## Amaç


MagicPIC'in temel amacı, PIC platformunu öğrenmek isteyen geliştiricilere anlaşılır, erişilebilir ve pratik kaynaklar sunmaktır.


Bu proje kapsamında;




•PIC16F886 tabanlı geliştirme kartlarının tasarlanması


•Temel ve gelişmiş çevre birimlerinin sürülmesi


•LCD, OLED, UART, SPI, I²C, ADC, PWM ve Timer örnekleri


•Modüler sürücü kütüphaneleri


•Örnek projeler


•Donanım şemaları


•Teknik dokümantasyon




geliştirilecektir.


## Proje Durumu


MagicPIC aktif geliştirme aşamasındadır.


İlk donanım prototiplerinin hazırlanmasının ardından örnek uygulamalar ve sürücü kütüphaneleri düzenli olarak depoya eklenecektir. Projenin temel hedeflerinden biri, PIC16F886 için Türkçe kaynak eksikliğini gidermek ve geliştiricilerin hızlı bir şekilde çalışmaya başlayabileceği kapsamlı bir referans oluşturmaktır.


## Yol Haritası




PIC16F886 geliştirme kartı


GPIO sürücüleri


Timer modülleri


ADC örnekleri


UART haberleşmesi


SPI ve I²C sürücüleri


LCD ve OLED örnekleri


Bootloader araştırmaları


Gelişmiş çevre birimi örnekleri


Donanım ve yazılım dokümantasyonu




Katkıda Bulunma


## Katkıda Bulunma

MagicPIC açık kaynaklı bir projedir.

Hata bildirimleri, öneriler, dokümantasyon katkıları ve yeni örnek projeler her zaman memnuniyetle karşılanmaktadır.

## Lisans

Bu proje MIT Lisansı altında lisanslanmıştır.  
Detaylar için `LICENSE` dosyasına bakabilirsiniz.

## Maintainer

**Barbaror4**

# FAQ (Sıkça Sorulan Sorular)

## Proje ismi nereden geldi?

MagicPIC ismi, **ProCenter Elektronik** tarafından üretilen ve geçmişte kullanılan **MagicFINDER** serisi cihazlardan esinlenerek oluşturulmuştur.

## Neden MagicFINDER?

Bu projenin isim hikayesi aslında bir elektronik tamir macerasına dayanıyor.

Babamın uzun yıllardır kullandığı **MagicFINDER Plus 3** uydu ölçüm cihazı, Şubat aylarında arızalandı. Elektroniğe olan ilgim nedeniyle cihazı inceleyip tamir etmeye karar verdim.

İlk incelemelerde sorunun klavye tarafında, özellikle zebra konektör veya keypad bağlantılarında olduğunu düşündüm. Çeşitli jumper ve kablo bağlantıları ile denemeler yapmama rağmen cihazın tuşları düzgün çalışmadı. Sadece bazı tuşlar, özellikle **DOWN** ve güç tuşu çalışıyordu.

10 Temmuz tarihinde cihazı tekrar incelemeye karar verdim. Bu sefer doğrudan cihazın ana işlemcisi olan **PIC18F4520** mikrodenetleyicisine odaklandım.

Keypad matris bağlantılarını ve işlemci pinlerini kontrol ettiğimde sorunun işlemcinin **RE1 pini** üzerinde olduğunu tespit ettim. Yapılan ölçümlerde:

- RE1 pininin VDD hattına kısa devre yaptığı
- Yaklaşık **3.51kΩ** seviyesinde kaçak direnç bulunduğu
- Dahili ESD koruma yapısının hasar görmüş olabileceği

belirlendi.

Ne yazık ki bu durum mikrodenetleyicinin silikon seviyesinde oluşmuş bir arızaya işaret ediyordu. RE1 pini izole edildiğinde ise cihazın güç kontrol süreci düzgün çalışmadığı için cihaz tamamen normal şekilde başlayamıyordu.

Bu durum benim için biraz üzücüydü. Elektronik cihazlara sadece bir eşya olarak değil, içerisinde yıllarca çalışan bir tasarım ve emek bulunan sistemler olarak bakıyorum. Bu cihaz ailemizde 16 yıldan uzun süredir kullanılıyordu ve tüm donanımı; ekranı, tuner bölümü ve diğer sistemleri hâlâ çalışır durumdayken, yalnızca tek bir mikrodenetleyici pini nedeniyle işlevlerinin büyük kısmını kaybetmesi oldukça ilginç bir deneyimdi.

Bu deneyimden sonra eski ve klasik mikrodenetleyicilere, özellikle **PIC ailesine** olan ilgim daha da arttı. MagicPIC projesi de bu merakın ve öğrenme sürecinin bir sonucu olarak ortaya çıktı.
