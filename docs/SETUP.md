# Setup and Configuration Guide

## Prerequisites

### For Arduino Development
- Arduino IDE (v1.8.0 or later)
- NodeMCU v3 (ESP8266-12E) or compatible board
- USB Cable (Micro-USB)
- Arduino libraries:
  - ESP8266WiFi
  - ESP8266HTTPClient
  - ArduinoJson

### For Web Dashboard
- Any modern web browser (Chrome, Firefox, Safari, Edge)
- Firebase project (free tier is sufficient)
- Text editor for configuration

### Hardware Requirements
- NodeMCU (ESP8266)
- Digital Soil Moisture Sensor
- 5V Relay Module
- Water pump (with power supply)
- Connecting wires and breadboard

## Step 1: Arduino IDE Setup

### 1.1 Install Arduino IDE
- Download from https://www.arduino.cc/en/software
- Install on your computer

### 1.2 Add ESP8266 Board
1. Open Arduino IDE
2. Go to **File → Preferences**
3. In "Additional Boards Manager URLs", add:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
4. Click **Tools → Board → Board Manager**
5. Search for "ESP8266" and install
6. Select **Tools → Board → NodeMCU 1.0 (ESP-12E)**

### 1.3 Install Required Libraries
1. Go to **Sketch → Include Library → Manage Libraries**
2. Search and install:
   - **ArduinoJson** by Benoit Blanchon (latest version)
   - Other libraries should already be included

## Step 2: Configure Arduino Project

### 2.1 Create Configuration Files

#### WiFi Configuration (`hardware/wifi_config.h`)
1. Open `hardware/wifi_config.h` in a text editor
2. Replace the placeholders:
   ```cpp
   #define WIFI_SSID "YOUR_WIFI_NAME"              // Your WiFi network name
   #define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"      // Your WiFi password
   ```
3. Save the file

**Example:**
```cpp
#define WIFI_SSID "Infinix HOT 30i"
#define WIFI_PASSWORD "9824056360"
```

#### Firebase Configuration (`hardware/firebase_config.h`)
1. Get your Firebase credentials:
   - Go to https://console.firebase.google.com/
   - Select your project
   - Click ⚙️ → Project Settings
   - Find "Realtime Database"
   - Copy the database URL (without `https://` and `/.json`)
   - Go to "Database Rules" → Copy your Database Secret

2. Open `hardware/firebase_config.h` and update:
   ```cpp
   #define FIREBASE_HOST "YOUR_FIREBASE_HOST"         // e.g., smart-plant-s-d17fd-default-rtdb.firebaseio.com
   #define FIREBASE_AUTH "YOUR_FIREBASE_AUTH_TOKEN"   // Your Database Secret
   ```
3. Save the file

**Example:**
```cpp
#define FIREBASE_HOST "smart-plant-irrigation-s-d17fd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "27EjMEy7Jje4hb6bscPI228LiQHQbxpAScX1IMGJ"
```

### 2.2 Upload to NodeMCU

1. Connect NodeMCU to computer via USB cable
2. In Arduino IDE:
   - **Tools → Port** → Select the COM port (usually `COM3` or `COM4`)
   - **Tools → Upload Speed** → Select `115200`
3. Open `hardware/sensor_code.ino`
4. Click **Upload** (→ button)
5. Wait for "Uploading..." message and then "Done uploading"

### 2.3 Verify Arduino Connection

1. Click **Tools → Serial Monitor**
2. Set baud rate to `115200`
3. You should see output like:
   ```
   IoT Smart Plant Irrigation System Starting...
   Connecting to WiFi: .....
   [OK] WiFi connected!
   IP Address: 192.168.x.x
   System ready! Waiting for sensor data...
   ```

If you see errors:
- Check WiFi SSID and password
- Ensure network is 2.4GHz (ESP8266 doesn't support 5GHz)
- Check Firebase credentials

## Step 3: Setup Firebase Project

### 3.1 Create Firebase Project (if not already created)
1. Go to https://console.firebase.google.com/
2. Click "Create Project"
3. Enter project name and follow setup wizard
4. Select "Realtime Database" and create in your region

### 3.2 Initialize Database Structure
1. Go to **Database → Rules**
2. Replace with:
   ```json
   {
     "rules": {
       "soilData": {
         ".read": true,
         ".write": true
       }
     }
   }
   ```
3. Click "Publish"

⚠️ **Note**: This allows public access. For production, implement proper authentication.

### 3.3 Create Initial Data Structure
1. Go to **Data** tab
2. Click the **+** button to add data
3. Create structure:
   ```
   soilData
     ├── moisture: 0
     ├── status: "Dry"
     ├── pump: "OFF"
     └── timestamp: 0
   ```

## Step 4: Configure Web Dashboard

### 4.1 Create Firebase Config for Web
1. Open `web-dashboard/js/firebase-config.js`
2. Get your Firebase config from Firebase console:
   - Go to Project Settings (⚙️)
   - Scroll to "Your apps"
   - Click Web app (if not created, create it first)
   - Copy the entire `firebaseConfig` object

3. Replace the placeholder in `firebase-config.js`:
   ```javascript
   const firebaseConfig = {
       apiKey: "AIzaSyDn7xX2f0kJvmHMjzU_i5WrUkQ4bAkuU_w",
       authDomain: "smart-plant-irrigation-s-d17fd.firebaseapp.com",
       databaseURL: "https://smart-plant-irrigation-s-d17fd-default-rtdb.firebaseio.com",
       projectId: "smart-plant-irrigation-s-d17fd",
       storageBucket: "smart-plant-irrigation-s-d17fd.firebasestorage.app",
       messagingSenderId: "669962291376",
       appId: "1:669962291376:web:453041a4d71f629d24d375",
       measurementId: "G-NGZ074DQM4"
   };
   ```

4. Save the file

### 4.2 Access the Dashboard
1. Open `web-dashboard/index.html` in a web browser
2. You should see:
   - Four data cards (Soil Moisture, Soil Condition, Pump Status, Last Updated)
   - "Water Plant" button for manual control
   - Moisture level trend chart

3. Wait for Arduino to send data (every 10 seconds)
4. Dashboard will display real-time sensor readings

## Step 5: Test the System

### 5.1 Verify Data Flow
1. **Check Arduino Serial Monitor**:
   - Should show sensor readings every 10 seconds
   - Should show Firebase connection status

2. **Check Web Dashboard**:
   - Cards should update with data
   - Chart should show moisture trend

3. **Check Firebase Console**:
   - Go to Database → Data
   - Should see `soilData` being updated with timestamps

### 5.2 Test Manual Pump Control
1. Click "Water Plant" button on dashboard
2. Check Arduino Serial Monitor - should show:
   ```
   Manual pump override: ON
   Pump turned ON by manual override
   ```
3. Relay should activate (you'll hear a click)
4. Click "Stop Watering" to turn off

### 5.3 Hardware Testing
- Apply moisture to sensor:
  - Should read as "Wet" (moisture = 100)
  - Pump should turn OFF automatically
- Remove moisture:
  - Should read as "Dry" (moisture = 0)
  - Pump should turn ON automatically

## Troubleshooting

### Arduino Issues

**Problem**: "Failed to open port"
- **Solution**: Check USB cable, try different USB port, restart Arduino IDE

**Problem**: "WiFi connection failed"
- **Solution**: 
  - Verify SSID and password in `wifi_config.h`
  - Ensure WiFi is 2.4GHz (not 5GHz)
  - Check WiFi password for special characters

**Problem**: "HTTP Error" in Serial Monitor
- **Solution**:
  - Verify Firebase credentials in `firebase_config.h`
  - Check Firebase database URL format
  - Ensure Firebase rules allow read/write

### Web Dashboard Issues

**Problem**: Dashboard shows "--" for all values
- **Solution**:
  - Check Firebase config in `firebase-config.js`
  - Open browser console (F12) for errors
  - Verify Arduino is sending data

**Problem**: "Cannot import module" error
- **Solution**:
  - Open dashboard through a local web server (not file://)
  - Use Python: `python -m http.server`
  - Or use VS Code Live Server extension

**Problem**: Button doesn't work
- **Solution**:
  - Check browser console for errors
  - Verify Firebase database rules allow write
  - Check network connection

## Configuration File Locations

| File | Location | Purpose |
|------|----------|---------|
| `wifi_config.h` | `hardware/` | WiFi credentials |
| `firebase_config.h` | `hardware/` | Arduino Firebase auth |
| `firebase-config.js` | `web-dashboard/js/` | Web Firebase config |

## Security Notes

⚠️ **Important**:
- These configuration files contain sensitive information
- **DO NOT commit them to git** (see `.gitignore`)
- **DO NOT share these files publicly**
- In production, use:
  - Environment variables
  - Secure backend services
  - Firebase authentication rules

## Next Steps

1. ✅ Arduino code uploaded and running
2. ✅ Firebase configured and receiving data
3. ✅ Web dashboard connected and displaying data
4. ✅ Manual pump control working

Your Smart Irrigation System is now ready to use!

## Additional Resources

- [Arduino ESP8266 Documentation](https://arduino-esp8266.readthedocs.io/)
- [Firebase Realtime Database](https://firebase.google.com/docs/database)
- [Chart.js Documentation](https://www.chartjs.org/docs/latest/)
- [Firebase Web SDK](https://firebase.google.com/docs/web/setup)
