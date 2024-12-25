/*
 * Project: İşletim Sistemleri Projesi(Operating Systems Project)
 * Contributors:
 *  - Talha Biçer
 *  - Talha iris
 *  - Ayberk Karaköse
 *  - Uygar Kesin
 *  - Cengizhan Keyfli
 *
 * Created: 2024
 */

#include "Shell.h"

int main() {

    pthread_t monitor_thread;

    // Arka planda çalışan işlemleri izleyen thread'i başlat
    if (pthread_create(&monitor_thread, NULL, monitor_background_processes, NULL) != 0) {
        perror("Failed to create monitoring thread");
        return 1;
    }

    // Shell fonksiyonunu başlat
    shell();

    // Thread'i bekle
    pthread_join(monitor_thread, NULL);

    return 0;
}