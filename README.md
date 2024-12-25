# İşletim Sistemleri Projesi (Operating Systems Project)

Bu proje, işletim sistemleri dersi kapsamında temel bir kabuk (shell) uygulaması geliştirmeyi amaçlar. Projede süreç yönetimi, I/O yönlendirme, borulama (pipe) ve Linux signal kullanımının temelleri öğrenilecektir. Kabuk uygulaması, kullanıcı komutlarını okuyup işleyerek çalıştıran bir komut satırı yorumlayıcısıdır.

---

## 📋 İçindekiler
- [Proje Hakkında](#proje-hakkında)
- [Özellikler](#özellikler)
- [Kurulum ve Çalıştırma](#kurulum-ve-çalıştırma)
- [Katkıda Bulunanlar](#katkıda-bulunanlar)
- [Lisans](#lisans)

---

## 📖 Proje Hakkında

Bu projede geliştirilecek olan kabuk, temel olarak şu işlemleri gerçekleştirecektir:
- Kullanıcıdan komut almak ve standart çıktıya çıktı vermek.
- I/O yönlendirme işlemlerini desteklemek.
- Borulama (pipe) işlemlerini gerçekleştirmek.
- Arkaplanda çalışan komutları yönetmek.
- Çeşitli yerleşik (built-in) komutlar ile etkileşim sağlamak.

---

## ✨ Özellikler

Projenin temel özellikleri şunlardır:

- **Prompt Gösterimi**: Her işlemden sonra komut istemi görüntülenir.
- **Yerleşik Komutlar**: `quit` gibi yerleşik komutlar çalıştırılabilir.
- **Tekli Komut İcrası**: Bağımsız argümanlarla tek komutlar çalıştırılır.
- **Giriş ve Çıkış Yönlendirme**: Dosyalar üzerinden giriş ve çıkış yönlendirme yapılabilir.
- **Arkaplan Çalışma**: İşlemler arkaplanda çalıştırılabilir.
- **Boru (Pipe)**: Komutların çıktıları, diğer komutların girdisi olarak kullanılabilir.

---

## 🚀 Kurulum ve Çalıştırma

### Gereksinimler
- Linux İşletim Sistemi
- C Derleyicisi (ör. GCC)

### Kurulum
1. Bu projeyi klonlayın:
   ```bash
   git clone https://github.com/talhairis/os_project.git
   cd os_project

2. Projeyi derlermek için makefile kullanın:
   make

---

## 👥 Katkıda Bulunanlar
- Talha Biçer
- Talha İris
- Ayberk Karaköse
- Uygar Kesin
- Cengizhan Keyfli

---

## 📜 Lisans
Bu proje yalnızca eğitim amaçlı geliştirilmiştir. Proje üzerindeki tüm haklar grup üyelerine aittir. İzinsiz kopyalanması veya dağıtılması yasaktır.
