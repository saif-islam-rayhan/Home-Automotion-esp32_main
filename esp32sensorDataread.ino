#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include "tone.h"

// ---------------- WIFI ----------------
#define WIFI_SSID "TP-Link_264C"
#define WIFI_PASSWORD "Sky12345"

// ---------------- FIREBASE ----------------
#define API_KEY "AIzaSyAxHjFOwi-_8MYHi0mwsHid6VBSSLHghNo"
#define DATABASE_URL "https://dht22iot-5b3fd-default-rtdb.firebaseio.com/"
#define USER_EMAIL "tuhinmia.ai42@gmail.com"
#define USER_PASSWORD "1234tuhi"

// ---------------- SENSOR ----------------
#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// ---------------- PINS ----------------
int irPin = 15;
int buzzer = 13;
int ledPin = 14;
int fanPin = 12;
int mq5Pin = 32;

// ---------------- FIREBASE ----------------
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ---------------- FAN PWM ----------------
int fanChannel = 0;
int freq = 5000;
int resolution = 8;

// ---------------- TIMER ----------------
unsigned long lastSend = 0;
unsigned long interval = 1000;

void setup() {

  Serial.begin(115200);

  pinMode(irPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Fan PWM
  ledcSetup(fanChannel, freq, resolution);
  ledcAttachPin(fanPin, fanChannel);

  // WiFi
  Serial.print("Connecting WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  // Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  dht.begin();
}

void loop() {

  if (millis() - lastSend >= interval) {

    lastSend = millis();

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int ir = digitalRead(irPin);
    int gas = analogRead(mq5Pin);

    if (isnan(temp) || isnan(hum)) {
      Serial.println("DHT Error");
      return;
    }

    Serial.printf("Temp: %.1f | Hum: %.1f | Gas: %d\n", temp, hum, gas);

    // ---------------- FIREBASE ----------------
    if (Firebase.ready()) {

      Firebase.RTDB.setFloat(&fbdo, "/sensor/temp", temp);
      Firebase.RTDB.setFloat(&fbdo, "/sensor/hum", hum);
      Firebase.RTDB.setInt(&fbdo, "/sensor/ir", ir);
      Firebase.RTDB.setInt(&fbdo, "/sensor/gas", gas);

      // LED control
      if (Firebase.RTDB.getInt(&fbdo, "/home/led")) {
        digitalWrite(ledPin, fbdo.intData());
      }

      // FAN control
      if (Firebase.RTDB.getInt(&fbdo, "/home/fan")) {
        int fanVal = fbdo.intData();
        ledcWrite(fanChannel, map(fanVal, 0, 100, 0, 255));
      }
    }

    // ---------------- IR SOUND ----------------
    if (ir == LOW) {
      playTitanic(buzzer);
    }

    // ---------------- GAS ALERT ----------------
    if (gas > 800) {
      tone(buzzer, 1000);
      delay(200);
      noTone(buzzer);
    }
  }
}