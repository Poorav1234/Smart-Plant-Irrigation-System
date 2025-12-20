# IoT Smart Plant Irrigation System - Arduino Setup

## Hardware Requirements:
1. ESP8266 (NodeMCU or similar)
2. Analog Soil Moisture Sensor
3. Relay Module (5V)
4. Water Pump (5V-12V)
5. Jumper wires
6. Breadboard

## Wiring Diagram:
```
ESP8266 (NodeMCU)     Components
-----------------     ----------
A0                   -> Soil Moisture Sensor AO pin
D6                   -> Relay Module IN pin
3.3V                 -> Soil Moisture Sensor VCC
GND                  -> Soil Moisture Sensor GND
5V                   -> Relay Module VCC
GND                  -> Relay Module GND
Relay NO (Normally Open) -> Water Pump Positive Terminal
Relay COM (Common)       -> Power Supply Positive Terminal
Pump Negative Terminal    -> Power Supply Negative Terminal
```

## Software Setup:

### 1. Arduino IDE Setup:
1. Install Arduino IDE
2. Install ESP8266 board package:
   - Go to File > Preferences
   - Add this URL to "Additional Board Manager URLs":
     `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - Go to Tools > Board > Boards Manager
   - Search for "esp8266" and install

### 2. Required Libraries:
Install these libraries through Library Manager (Sketch > Include Library > Manage Libraries):
1. FirebaseArduino
2. ESP8266WiFi

### 3. Firebase Setup:
1. Go to Firebase Console (https://console.firebase.google.com/)
2. Create a new project or use existing "smart-plant-irrigation-s-d17fd"
3. Go to Project Settings > Service Accounts > Database Secrets
4. Copy the database secret key

### 4. WiFi Setup:
1. Replace "your_wifi_ssid" and "your_wifi_password" with your WiFi credentials
2. Replace "your_firebase_secret_key" with your Firebase database secret

## Calibration Instructions:
1. Upload the sketch to your ESP8266
2. Open Serial Monitor (Tools > Serial Monitor)
3. With sensor in air, note the value (should be around 700-800)
4. With sensor in water, note the value (should be around 200-300)
5. Update AIR_VALUE and WATER_VALUE in the code with your measured values

## Troubleshooting:
1. If getting constant 100% or 0% readings, check wiring
2. If Firebase connection fails, check credentials
3. If relay doesn't activate, check power supply to pump
4. If WiFi doesn't connect, check SSID/password

## Data Structure in Firebase:
The Arduino will populate the following structure in your Firebase Realtime Database:
```
{
  "soilData": {
    "moisture": 45,
    "status": "Ideal",
    "pump": "OFF",
    "timestamp": 1234567890
  }
}
```

This matches the structure expected by your React dashboard.