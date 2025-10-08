#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h> // Library lama sesuai jobsheet
#include <addons/TokenHelper.h>
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
#define DHTPIN 4 // Pin D4 di NodeMCU ESP8266
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Objek-objek Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

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
    Serial.println("\nTerhubung dengan IP: " + WiFi.localIP().toString());
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    // --- Konfigurasi Firebase (Dipindahkan ke dalam setup) ---
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
    if (Firebase.ready()) {
        delay(10000); // Tunggu 10 detik

        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Gagal membaca dari sensor DHT!");
            return;
        }

        Serial.printf("Suhu: %.2f °C, Kelembaban: %.2f %%\n", temperature, humidity);

        // Path dokumen: koleksi 'EspData' -> dokumen 'DHT11' (sesuai jobsheet)
        String documentPath = "EspData/DHT11";
        
        // Buat konten JSON untuk dikirim
        String content = "{\"fields\": {\"Temperature\": {\"stringValue\": \"" + String(temperature, 2) + "\"}, \"Humidity\": {\"stringValue\": \"" + String(humidity, 2) + "\"}}}";

        Serial.println("Mengirim data ke Firestore...");
        
        // Kirim data menggunakan patchDocument
        if (Firebase.Firestore.patchDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), content.c_str(), "")) {
            Serial.println("BERHASIL: Data terkirim.");
        } else {
            Serial.printf("GAGAL: %s\n", fbdo.errorReason().c_str());
        }
    }
}