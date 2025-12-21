// IoT Smart Plant Irrigation System - Firebase + NodeMCU
// Works with digital soil moisture sensor (non-blocking updates)

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// WiFi credentials
#define WIFI_SSID "vivo T2 5G"
#define WIFI_PASSWORD "21002100"

// Firebase configuration
#define FIREBASE_HOST "smart-plant-irrigation-s-d17fd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH ""  // Database secret from Firebase

// Pin definitions
#define SOIL_PIN D5    // Digital soil moisture sensor pin
#define RELAY_PIN D6   // Relay control pin
#define LED_PIN 2      // Built-in LED

// Timing
const unsigned long UPDATE_INTERVAL = 10000; // 10 seconds
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  delay(2000); // Give Serial Monitor time to open

  pinMode(SOIL_PIN, INPUT);        // Digital sensor input
  pinMode(RELAY_PIN, OUTPUT);      // Relay output
  pinMode(LED_PIN, OUTPUT);        // Built-in LED

  // Start with relay OFF
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("IoT Smart Plant Irrigation System Starting...");

  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to Firebase
  Serial.println("Connecting to Firebase...");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if (Firebase.failed()) {
    Serial.print("Firebase connection failed: ");
    Serial.println(Firebase.error());
    return;
  }
  Serial.println("Firebase connected successfully!");
  Serial.println("----------------------------------------");
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= UPDATE_INTERVAL) {
    previousMillis = currentMillis;
    readAndSendSensorData();
  }
}

void readAndSendSensorData() {
  // Read digital sensor (HIGH = dry, LOW = wet)
  int soilValue = digitalRead(SOIL_PIN);
  Serial.print("Soil Sensor Value: ");
  Serial.println(soilValue);

  int moisturePercentage;
  String soilCondition;

  if (soilValue == HIGH) {
    // Dry soil → pump ON
    moisturePercentage = 0;
    soilCondition = "Dry";
    digitalWrite(RELAY_PIN, LOW);   // Activate pump
    digitalWrite(LED_PIN, LOW);     // LED ON
    Serial.println("Soil is DRY - Activating pump");
  } else {
    // Wet soil → pump OFF
    moisturePercentage = 100;
    soilCondition = "Wet";
    digitalWrite(RELAY_PIN, HIGH);  // Deactivate pump
    digitalWrite(LED_PIN, HIGH);    // LED OFF
    Serial.println("Soil is WET - Deactivating pump");
  }

  sendDataToFirebase(moisturePercentage, soilCondition);

  Serial.print("Waiting ");
  Serial.print(UPDATE_INTERVAL / 1000);
  Serial.println(" seconds before next update...");
  Serial.println("----------------------------------------");
}

void sendDataToFirebase(int moisture, String condition) {
  unsigned long timestamp = millis() / 1000;

  Serial.print("Sending data to Firebase: Moisture=");
  Serial.print(moisture);
  Serial.print("%, Condition=");
  Serial.println(condition);

  Firebase.setFloat("soilData/moisture", moisture);
  Firebase.setString("soilData/status", condition);

  int relayStatus = digitalRead(RELAY_PIN);
  String pumpStatus = (relayStatus == LOW) ? "ON" : "OFF";
  Firebase.setString("soilData/pump", pumpStatus);
  Firebase.setInt("soilData/timestamp", (int)timestamp);

  if (Firebase.failed()) {
    Serial.print("Firebase error: ");
    Serial.println(Firebase.error());
  } else {
    Serial.println("Data sent successfully!");
  }
}
