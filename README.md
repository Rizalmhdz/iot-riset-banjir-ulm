# BOARD ESP32
- Lib : https://dl.espressif.com/dl/package_esp32_index.json

# LED NOTIF NOTES

## 3 detik pertama indikator SDCard adaptor:
- Jika LED KUNING menyala selama 3 detik maka SDCard terbaca oleh sistem.
- Jika LED KUNING dan LED MERAH hidup mati selama 3 detik (6 kali hidup mati, 0.25 detik interval) maka SDCard tidak terbaca dan data tidak akan tersimpan.

## 3 detik selanjutnya indikator RTC (RealTime Clock):
- Jika LED HIJAU menyala selama 3 detik maka RTC aktif/terdeteksi.
- Jika LED HIJAU hidup mati bersama LED MERAH selama 3 detik (6 kali hidup mati, 0.25 detik interval) maka RTC tidak terdeteksi dan sistem akan restart.

## Indikator Waktu Jam dan Menit:
- Indikator waktu saat setup:
  - LED MERAH menyala selama 1 detik lalu mati selama 0,5 detik.
  - Nyalakan LED HIJAU untuk puluhan jam dimulai dari 1:
    - Setiap hitungan puluhan jam, LED HIJAU menyala selama 0,5 detik lalu mati selama 0,5 detik.
  - Nyalakan LED KUNING untuk satuan jam dimulai dari 1:
    - Setiap hitungan satuan jam, LED KUNING menyala selama 0,5 detik lalu mati selama 0,5 detik.
  - Nyalakan LED HIJAU untuk puluhan menit dimulai dari 0:
    - Setiap hitungan puluhan menit, LED HIJAU menyala selama 0,5 detik lalu mati selama 0,5 detik.
  - Nyalakan LED KUNING untuk satuan menit dimulai dari 0:
    - Setiap hitungan satuan menit, LED KUNING menyala selama 0,5 detik lalu mati selama 0,5 detik.

## Indikator Tip Counter:
- Jika saklar reed aktif (tip terdeteksi):
  - LED MERAH dan LED HIJAU menyala selama 0,25 detik lalu mati selama 0,25 detik (1 kali).
- Jika saklar reed tidak aktif:
  - LED tidak menyala.

## Indikator Reset Counter:
- Reset tip counter setiap 1 jam:
  - LED MERAH, LED KUNING, dan LED HIJAU menyala selama 0,25 detik lalu mati selama 0,25 detik, diulang 6 kali (total 3 detik).

## Indikator Penulisan ke SDCard:
- Penulisan data berhasil:
  - LED HIJAU dan LED KUNING menyala selama 0,25 detik lalu mati selama 0,25 detik, diulang 3 kali (total 1,5 detik).
- Penulisan data gagal:
  - LED MERAH dan LED KUNING menyala selama 0,25 detik lalu mati selama 0,25 detik, diulang 3 kali (total 1,5 detik).
- Pembacaan data gagal:
  - LED MERAH dan LED KUNING menyala selama 0,25 detik lalu mati selama 0,25 detik, diulang 6 kali (total 3 detik).

## Indikator Sensor Ultrasonik:
- Mengukur jarak menggunakan sensor ultrasonik:
  - Hasil pengukuran ditampilkan di Serial Monitor tanpa indikasi LED tambahan.

## Indikator Sensor HC-SR04:
- Mengukur jarak menggunakan sensor HC-SR04:
  - Hasil pengukuran ditampilkan di Serial Monitor tanpa indikasi LED tambahan.
