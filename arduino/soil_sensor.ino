// IoT Smart Plant Irrigation System
// Modified to provide percentage moisture readings and send data to Firebase

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// WiFi credentials - UPDATE THESE WITH YOUR NETWORK CREDENTIALS
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// Firebase credentials - UPDATE THESE WITH YOUR FIREBASE CREDENTIALS
#define FIREBASE_HOST "smart-plant-irrigation-s-d17fd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "your_firebase_secret_key"

// Pin definitions
#define SOIL_PIN A0    // Soil moisture sensor pin (analog) - CHANGE TO A0 FOR ANALOG READING
#define RELAY_PIN D6   // Relay control pin (connect to IN pin of relay)
#define LED_PIN 2      // Built-in LED (for debug)

// Calibration values for soil moisture sensor - YOU WILL NEED TO CALIBRATE THESE
// These are typical values, but you should calibrate for your specific sensor
#define AIR_VALUE 750   // Sensor value in air (0% moisture)
#define WATER_VALUE 300 // Sensor value in water (100% moisture)

// Timing variables
unsigned long lastUpdate = 0;
const long updateInterval = 30000; // Update every 30 seconds (30000 ms)

void setup() {
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(SOIL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Start with relay off
  digitalWrite(RELAY_PIN, HIGH);  // HIGH = relay off for most modules
  digitalWrite(LED_PIN, HIGH);    // LED off (HIGH = off for built-in LED)
  
  Serial.println("IoT Smart Plant Irrigation System Initializing...");
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
  
  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println("Connected to Firebase");
  
  Serial.println("System Ready!");
  Serial.println("----------------------------------------");
}

void loop() {
  // Update data at regular intervals
  if (millis() - lastUpdate > updateInterval) {
    lastUpdate = millis();
    
    // Read analog soil moisture value
    int soilValue = analogRead(SOIL_PIN);
    
    // Convert to percentage (0-100%)
    // Map function converts from one range to another
    int moisturePercentage = map(soilValue, AIR_VALUE, WATER_VALUE, 0, 100);
    
    // Constrain the value between 0 and 100
    moisturePercentage = constrain(moisturePercentage, 0, 100);
    
    Serial.print("Raw Sensor Value: ");
    Serial.print(soilValue);
    Serial.print(" | Moisture Percentage: ");
    Serial.print(moisturePercentage);
    Serial.println("%");
    
    // Determine soil condition based on moisture percentage
    String soilCondition;
    if (moisturePercentage < 30) {
      soilCondition = "Dry";
    } else if (moisturePercentage > 70) {
      soilCondition = "Wet";
    } else {
      soilCondition = "Ideal";
    }
    
    Serial.print("Soil Condition: ");
    Serial.println(soilCondition);
    
    // Automatic irrigation control
    // Turn on pump if soil is dry
    if (moisturePercentage < 30) {
      Serial.println("Soil is dry - Activating irrigation pump!");
      digitalWrite(RELAY_PIN, LOW);   // LOW = activate relay (turn on pump)
      digitalWrite(LED_PIN, LOW);     // LED ON (LOW = on for built-in LED)
    } 
    // Turn off pump if soil is adequately moist
    else {
      Serial.println("Soil moisture is adequate - Deactivating irrigation pump");
      digitalWrite(RELAY_PIN, HIGH);  // HIGH = deactivate relay (turn off pump)
      digitalWrite(LED_PIN, HIGH);    // LED OFF
    }
    
    // Send data to Firebase
    sendDataToFirebase(moisturePercentage, soilCondition);
  }
}

void sendDataToFirebase(int moisture, String condition) {
  // Get Unix timestamp (seconds since Jan 1, 1970)
  // For simplicity, we'll use millis()/1000, but in production you'd use NTP
  unsigned long timestamp = millis() / 1000;
  
  // Create JSON object to send to Firebase
  Firebase.setFloat("soilData/moisture", moisture);
  Firebase.setString("soilData/status", condition);
  
  // Check relay status to determine pump status
  int relayStatus = digitalRead(RELAY_PIN);
  String pumpStatus = (relayStatus == LOW) ? "ON" : "OFF";
  Firebase.setString("soilData/pump", pumpStatus);
  
  Firebase.setUInt("soilData/timestamp", timestamp);
  
  // Check for errors
  if (Firebase.failed()) {
    Serial.print("Firebase update failed: ");
    Serial.println(Firebase.error());
    return;
  }
  
  Serial.println("Data successfully sent to Firebase");
  Serial.println("----------------------------------------");
}