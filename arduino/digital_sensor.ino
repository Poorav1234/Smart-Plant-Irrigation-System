// IoT Smart Plant Irrigation System - Updated Firebase Configuration
// Maps digital readings (0/1) to meaningful percentages for the website

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// WiFi credentials - UPDATE THESE WITH YOUR NETWORK CREDENTIALS
#define WIFI_SSID "BlackNamer"
#define WIFI_PASSWORD "12345678"

// Firebase configuration - USE DATABASE SECRET FROM FIREBASE CONSOLE
#define FIREBASE_HOST "smart-plant-irrigation-s-d17fd-default-rtdb.firebaseio.com"
// Get this from Firebase Console: Project Settings > Service Accounts > Database Secrets
#define FIREBASE_AUTH ""



// Pin definitions
#define SOIL_PIN D5    // Digital soil moisture sensor pin
#define RELAY_PIN D6   // Relay control pin
#define LED_PIN 2      // Built-in LED

void setup() {
  Serial.begin(9600);
  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Start with relay off
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH);
  
  Serial.println("IoT Smart Plant Irrigation System Starting...");
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
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
  Serial.println("System Ready - Sending data to website");
  Serial.println("----------------------------------------");
}

void loop() {
  // Read digital soil moisture value
  int soilValue = digitalRead(SOIL_PIN);
  
  // Map digital value to meaningful percentages
  // HIGH (dry) = 0%, LOW (wet) = 100%
  int moisturePercentage;
  String soilCondition;
  
  if (soilValue == HIGH) {
    // Dry soil
    moisturePercentage = 0;
    soilCondition = "Dry";
    // Turn on pump
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Soil is DRY - Activating pump");
  } else {
    // Wet soil
    moisturePercentage = 100;
    soilCondition = "Wet";
    // Turn off pump
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Soil is WET - Deactivating pump");
  }
  
  // Send data to Firebase for website display
  sendDataToFirebase(moisturePercentage, soilCondition);
  
  Serial.print("Waiting 10 seconds before next update...");
  for(int i = 0; i < 10; i++) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
}

void sendDataToFirebase(int moisture, String condition) {
  // Get timestamp (simplified for Arduino)
  unsigned long timestamp = millis() / 1000;
  
  Serial.print("Sending data: Moisture=");
  Serial.print(moisture);
  Serial.print("%, Condition=");
  Serial.print(condition);
  Serial.println("");
  
  // Send data to Firebase - this automatically updates your website
  Firebase.setFloat("soilData/moisture", moisture);
  Firebase.setString("soilData/status", condition);
  
  // Check relay status
  int relayStatus = digitalRead(RELAY_PIN);
  String pumpStatus = (relayStatus == LOW) ? "ON" : "OFF";
  Firebase.setString("soilData/pump", pumpStatus);
  
  Firebase.setUInt("soilData/timestamp", timestamp);
  
  if (Firebase.failed()) {
    Serial.print("Firebase error: ");
    Serial.println(Firebase.error());
    return;
  }
  
  Serial.println("Data sent to website successfully!");
  Serial.println("----------------------------------------");
}