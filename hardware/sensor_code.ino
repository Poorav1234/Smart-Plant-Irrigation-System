// IoT Smart Plant Irrigation System - Firebase + NodeMCU
// Works with digital soil moisture sensor (non-blocking updates)
// Using simple HTTP REST API (no special library needed)

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

// Include configuration files
#include "wifi_config.h"
#include "firebase_config.h"

// Pin definitions
#define SOIL_PIN D5    // Digital soil moisture sensor pin
#define RELAY_PIN D6   // Relay control pin
#define LED_PIN 2      // Built-in LED

// Timing
const unsigned long UPDATE_INTERVAL = 10000; // 10 seconds
unsigned long previousMillis = 0;

// HTTP client
WiFiClientSecure client;
HTTPClient http;

void setup() {
  Serial.begin(115200);  // Use 115200 for better reliability
  delay(2000);

  pinMode(SOIL_PIN, INPUT);        // Digital sensor input
  pinMode(RELAY_PIN, OUTPUT);      // Relay output
  pinMode(LED_PIN, OUTPUT);        // Built-in LED

  // Start with relay OFF
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);

  Serial.println("\n\nIoT Smart Plant Irrigation System Starting...");

  // Disable SSL certificate verification for Firebase (not recommended for production)
  client.setInsecure();

  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
    Serial.print(".");
    delay(500);
    wifiAttempts++;
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("[OK] WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("[FAIL] WiFi connection failed!");
    return;
  }

  Serial.println("System ready! Waiting for sensor data...");
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
    // Dry soil --> pump ON
    moisturePercentage = 0;
    soilCondition = "Dry";
    digitalWrite(RELAY_PIN, LOW);   // Activate pump
    digitalWrite(LED_PIN, LOW);     // LED ON
    Serial.println("Soil is DRY - Activating pump");
  } else {
    // Wet soil --> pump OFF
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

  // First, check for manual pump override
  checkAndApplyManualPumpOverride();

  int relayStatus = digitalRead(RELAY_PIN);
  String pumpStatus = (relayStatus == LOW) ? "ON" : "OFF";

  // Create JSON payload
  DynamicJsonDocument doc(256);
  doc["moisture"] = moisture;
  doc["status"] = condition;
  doc["pump"] = pumpStatus;
  doc["timestamp"] = (long)timestamp;

  String jsonString;
  serializeJson(doc, jsonString);

  // Build Firebase URL
  String url = "https://" + String(FIREBASE_HOST) + "/soilData.json?auth=" + String(FIREBASE_AUTH);

  // Send data to Firebase
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.PATCH(jsonString);
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    Serial.println("[OK] Data sent successfully!");
  } else {
    Serial.print("[ERROR] HTTP Error: ");
    Serial.println(httpCode);
    Serial.print("Response: ");
    Serial.println(http.getString());
  }
  
  http.end();
}

void checkAndApplyManualPumpOverride() {
  // Build Firebase URL to read manualPump
  String url = "https://" + String(FIREBASE_HOST) + "/soilData/manualPump.json?auth=" + String(FIREBASE_AUTH);

  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK || httpCode == 200) {
    String response = http.getString();
    
    // Remove quotes from JSON response
    response.trim();
    if (response.startsWith("\"") && response.endsWith("\"")) {
      response = response.substring(1, response.length() - 1);
    }
    
    if (response == "null" || response.isEmpty()) {
      Serial.println("No manual override set");
    } else {
      Serial.print("Manual pump override: ");
      Serial.println(response);
      
      if (response == "ON") {
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
        Serial.println("Pump turned ON by manual override");
      } else if (response == "OFF") {
        digitalWrite(RELAY_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Pump turned OFF by manual override");
      }
    }
  } else {
    Serial.print("[ERROR] Error reading manual override: ");
    Serial.println(httpCode);
  }
  
  http.end();
}
