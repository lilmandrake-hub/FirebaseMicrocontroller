# Proyek Monitoring Sensor DHT11 dengan ESP8266/ESP32 dan Google Firestore

Ingin belajar menghubungkan perangkat fisik ke cloud? Proyek ini adalah titik awal yang sempurna! Anda akan belajar cara membaca data dari sensor DHT11 dan mengirimkannya ke Google Firestore untuk monitoring real-time dari mana saja. Kami menyediakan dua contoh kode yang jelas untuk platform ESP8266 dan ESP32, lengkap dengan penjelasan untuk membantu Anda membangun proyek IoT pertama Anda.

Ini adalah proyek dasar yang sangat baik untuk memulai dengan Internet of Things (IoT), integrasi *cloud*, dan visualisasi data sensor.

## Fitur Utama 🚀
* **Logging Data Real-Time**: Mengirim data suhu dan kelembaban ke cloud setiap 10 detik.
* **Dukungan Multi-Platform**: Menyediakan kode terpisah untuk NodeMCU ESP8266 dan ESP32.
* **Database Cloud Scalable**: Menggunakan Google Firestore, sebuah database NoSQL yang andal untuk menyimpan data time-series.
* **Autentikasi Pengguna**: Mengamankan penulisan data ke database menggunakan sistem autentikasi email/password dari Firebase.
* **Dua Pendekatan Kode**: Menunjukkan dua metode penjadwalan yang berbeda:
    * **Blocking** (`delay()`) untuk kesederhanaan pada ESP8266.
    * **Non-Blocking** (`millis()`) untuk efisiensi pada ESP32.

---
## Kebutuhan Perangkat

### Perangkat Keras 🔌
* NodeMCU ESP8266 atau ESP32 Dev Kit
* Sensor Suhu & Kelembaban DHT11
* Kabel Jumper
* Kabel Micro USB
* Akses ke jaringan Wi-Fi

### Perangkat Lunak & Library 💻
1.  **Arduino IDE**
2.  **Board Manager**:
    * `esp8266` by ESP8266 Community.
    * `esp32` by Espressif Systems.
3.  **Arduino Libraries**:
    * `Firebase ESP Client` oleh Mobizt (untuk komunikasi dengan Firebase).
    * `DHT sensor library` oleh Adafruit (untuk membaca sensor DHT11).

---
## Konfigurasi

### 1. Pengaturan Proyek Firebase
Sebelum mengunggah kode, Anda perlu menyiapkan proyek di Firebase.

1.  **Buat Proyek**: Buka [Firebase Console](https://console.firebase.google.com/), klik **Tambahkan Proyek**, dan beri nama proyek Anda (misalnya, "DHT11 Firestore").
2.  **Aktifkan Autentikasi**:
    * Di menu sebelah kiri, buka `Build` > `Authentication`.
    * Klik **Mulai**, pilih `Email/Password` sebagai metode masuk, dan aktifkan.
    * Masuk ke tab **Users** dan klik **Add user** untuk membuat akun pengguna baru yang akan digunakan oleh ESP Anda.
3.  **Buat Firestore Database**:
    * Buka `Build` > `Firestore Database`.
    * Klik **Buat database** dan mulai dalam **mode pengujian (test mode)**.
4.  **Dapatkan Kredensial**:
    * Klik ikon gerigi di sebelah *Project Overview* dan pilih **Project settings**.
    * Di bawah tab *General*, salin **Web API Key** dan **Project ID**. Anda akan membutuhkannya di kode Arduino.

### 2. Pengaturan Kode Arduino
1.  Pilih file yang sesuai untuk board Anda:
    * `FirebaseESP8266V2DHT.ino` untuk ESP8266.
    * `FirebaseESP32V2DHT.ino` untuk ESP32.
2.  Buka file di Arduino IDE.
3.  Isi kredensial Anda pada baris-baris berikut:
    ```cpp
    // --- KREDENSIAL WIFI ---
    #define WIFI_SSID "NAMA_WIFI_ANDA"
    #define WIFI_PASSWORD "PASSWORD_WIFI_ANDA"

    // --- KREDENSIAL FIREBASE ---
    #define API_KEY "WEB_API_KEY_ANDA"
    #define FIREBASE_PROJECT_ID "ID_PROYEK_ANDA"

    // --- KREDENSIAL PENGGUNA ---
    #define USER_EMAIL "EMAIL_ANDA_YANG_DIDAFTARKAN"
    #define USER_PASSWORD "PASSWORD_ANDA"
    ```

---
## Diagram Rangkaian

Hubungkan sensor DHT11 ke board ESP Anda sebagai berikut. Pin data terhubung ke **D4 (GPIO 4)**.

| Pin DHT11 | Pin ESP8266 / ESP32 |
| :--- | :--- |
| **VCC / +** | `3V3` |
| **GND / -** | `GND` |
| **DATA / OUT** | `D4` |

---
## Cara Menggunakan

1.  Pastikan semua pengaturan Firebase dan kode Arduino sudah selesai.
2.  Rangkai perangkat keras sesuai diagram di atas.
3.  Hubungkan board ESP ke komputer Anda.
4.  Di Arduino IDE, pilih Board dan Port yang benar.
5.  Klik **Upload** untuk mengunggah kode.
6.  Buka **Serial Monitor** dan atur baud rate ke **115200**. Anda akan melihat log koneksi Wi-Fi dan status pengiriman data.
7.  Buka konsol Firebase, navigasi ke **Firestore Database**, dan Anda akan melihat data suhu dan kelembaban muncul dan diperbarui setiap 10 detik!

---
## Lisensi

Proyek ini dilisensikan di bawah Lisensi MIT.
