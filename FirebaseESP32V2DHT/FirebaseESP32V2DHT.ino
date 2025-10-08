#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Menyertakan helper untuk callback status token
#include <addons/TokenHelper.h>
// Menyertakan library DHT
#include <DHT.h>

// --- KREDENSIAL WIFI ---
#define WIFI_SSID "NAMA_WIFI_ANDA"
#define WIFI_PASSWORD "PASSWORD_WIFI_ANDA"

// --- KREDENSIAL FIREBASE ---
#define API_KEY "WEB_API_KEY_ANDA"
#define FIREBASE_PROJECT_ID "ID_PROYEK_ANDA"

// --- KREDENSIAL PENGGUNA ---
#define USER_EMAIL "EMAIL_ANDA_YANG_DIDAFTARKAN"
#define USER_PASSWORD "PASSWORD_ANDA"

// --- PENGATURAN SENSOR DHT11 ---
#define DHTPIN 4       // Pin GPIO 4 untuk data sensor DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Objek-objek Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;

void setup() {
    Serial.begin(115200);
    dht.begin(); // Inisialisasi sensor DHT11

    // Koneksi ke WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Menghubungkan ke Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Terhubung dengan IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    // Konfigurasi Firebase
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback;
    Firebase.reconnectNetwork(true);
    fbdo.setBSSLBufferSize(1024, 1024);
    fbdo.setResponseSize(2048);

    // Inisialisasi Firebase
    Firebase.begin(&config, &auth);
}

void loop() {
    // Cek jika Firebase siap dan sudah 10 detik berlalu
    if (Firebase.ready() && millis() - dataMillis > 10000) {
        dataMillis = millis();

        // Baca suhu dan kelembaban dari sensor
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        // Pastikan pembacaan sensor valid (bukan NaN - Not a Number)
        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Gagal membaca dari sensor DHT!");
            return; // Lewati iterasi loop ini jika data tidak valid
        }

        Serial.printf("Suhu: %.2f °C, Kelembaban: %.2f %%\n", temperature, humidity);

        // --- Mengirim Data Sensor ke Firestore ---

        // 1. Tentukan path ke dokumen. Kita akan update satu dokumen saja.
        String documentPath = "dataSensor/dht11";
        
        // 2. Buat konten JSON dengan field suhu dan kelembaban
        // Perhatikan tipe datanya adalah stringValue sesuai contoh jobsheet
        String content = "{\"fields\": {\"suhu\": {\"stringValue\": \"" + String(temperature, 2) + "\"}, \"kelembaban\": {\"stringValue\": \"" + String(humidity, 2) + "\"}}}";

        Serial.printf("Mengirim data ke Firestore path: %s\n", documentPath.c_str());

        // 3. Gunakan .patchDocument untuk membuat atau menimpa dokumen
        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.c_str(), "")) {
            Serial.printf("BERHASIL: Dokumen diupdate.\n%s\n", fbdo.payload().c_str());
        } else {
            Serial.printf("GAGAL: %s\n", fbdo.errorReason().c_str());
        }
    }
}