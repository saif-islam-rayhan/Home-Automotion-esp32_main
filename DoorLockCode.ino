// #include <WiFi.h>
// #include <Firebase_ESP_Client.h>
// #include <ESP32Servo.h>

// // -------- WIFI --------
// #define WIFI_SSID "TP-Link_264C"
// #define WIFI_PASSWORD "Sky12345"

// // -------- FIREBASE --------
// #define API_KEY "AIzaSyAxHjFOwi-_8MYHi0mwsHid6VBSSLHghNo"
// #define DATABASE_URL "https://dht22iot-5b3fd-default-rtdb.firebaseio.com/"
// #define USER_EMAIL "tuhinmia.ai42@gmail.com"
// #define USER_PASSWORD "1234tuhi"

// // -------- PINS --------
// int servoPin = 26;
// int buttonPin = 23;

// // -------- SERVO --------
// Servo doorServo;

// // -------- FIREBASE --------
// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// // -------- STATE --------
// bool doorOpen = false;
// unsigned long doorTimer = 0;

// void setup() {

//   Serial.begin(115200);

//   pinMode(buttonPin, INPUT_PULLUP);

//   // Servo init
//   doorServo.attach(servoPin, 500, 2400);
//   doorServo.write(0);

//   // WiFi
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(300);
//     Serial.print(".");
//   }

//   Serial.println("\nWiFi Connected");

//   // Firebase
//   config.api_key = API_KEY;
//   config.database_url = DATABASE_URL;

//   auth.user.email = USER_EMAIL;
//   auth.user.password = USER_PASSWORD;

//   Firebase.begin(&config, &auth);
//   Firebase.reconnectWiFi(true);
// }

// // -------- OPEN DOOR --------
// void openDoor() {

//   Serial.println("🚪 Door OPEN");

//   doorServo.write(120);
//   doorOpen = true;
//   doorTimer = millis();
// }

// // -------- CLOSE DOOR --------
// void closeDoor() {

//   Serial.println("🔒 Door CLOSE");

//   doorServo.write(0);
//   doorOpen = false;
// }

// // -------- LOOP --------
// void loop() {

//   // AUTO CLOSE after 5 sec
//   if (doorOpen && millis() - doorTimer >= 5000) {
//     closeDoor();
//   }

//   // -------- BUTTON CONTROL --------
//   if (digitalRead(buttonPin) == LOW && !doorOpen) {

//     openDoor();
//     delay(300); // debounce
//   }

//   // -------- FIREBASE CONTROL --------
//   if (Firebase.ready()) {

//     if (Firebase.RTDB.getInt(&fbdo, "/home/door")) {

//       int cmd = fbdo.intData();

//       if (cmd == 1 && !doorOpen) {

//         openDoor();

//         Firebase.RTDB.setInt(&fbdo, "/home/door", 0);
//       }
//     }
//   }

//   delay(50);
// }