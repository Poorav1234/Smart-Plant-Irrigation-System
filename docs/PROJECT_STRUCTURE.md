# Smart Plant Irrigation System - Project Structure

## Overview
This is a complete IoT project for automated plant irrigation with real-time monitoring using NodeMCU (ESP8266) and Firebase Realtime Database.

## Directory Structure

```
Smart-Plant-Irrigation-System/
│
├── hardware/                        # Arduino/NodeMCU Code
│   ├── sensor_code.ino             # Main Arduino application
│   ├── wifi_config.h               # WiFi credentials (EXCLUDED from git)
│   └── firebase_config.h           # Firebase credentials (EXCLUDED from git)
│
├── web-dashboard/                  # Web Frontend
│   ├── index.html                  # Main dashboard page
│   ├── css/
│   │   └── styles.css              # Dashboard styling
│   └── js/
│       ├── app.js                  # Main application logic
│       └── firebase-config.js      # Firebase config (EXCLUDED from git)
│
├── config/                         # Configuration templates
│   ├── wifi_config.h.example       # WiFi config template
│   └── firebase_config.js.example  # Firebase config template
│
├── docs/                           # Documentation
│   ├── SETUP.md                    # Setup and installation guide
│   ├── PROJECT_STRUCTURE.md        # This file
│   ├── API_REFERENCE.md            # Firebase database schema
│   └── HARDWARE_GUIDE.md           # Hardware connections
│
├── .gitignore                      # Git ignore file (excludes credentials)
├── README.md                       # Project overview
└── .git/                           # Git repository
```

## Key Features

### 1. **Hardware Module** (`hardware/`)
- **sensor_code.ino**: Main Arduino sketch for NodeMCU (ESP8266)
  - Reads digital soil moisture sensor
  - Controls relay-based irrigation pump
  - Communicates with Firebase via REST API
  - Supports manual pump override from web dashboard

- **Configuration Files**:
  - `wifi_config.h`: WiFi network credentials
  - `firebase_config.h`: Firebase database authentication

### 2. **Web Dashboard** (`web-dashboard/`)
- **index.html**: Clean, responsive HTML structure
  - Dashboard cards for real-time data display
  - Chart for moisture level trends
  - Manual watering control button

- **css/styles.css**: Professional styling
  - Gradient background with animations
  - Responsive grid layout
  - Status-based color coding (Dry/Wet)
  - Mobile-friendly design

- **js/app.js**: Main application logic
  - Firebase initialization and connection
  - Real-time data listeners
  - Chart updates using Chart.js
  - Button event handlers

- **js/firebase-config.js**: Firebase credentials (template)
  - API keys and database URLs
  - NOT committed to git (see .gitignore)

### 3. **Configuration Files** (`config/`)
- Example/template files for easy setup
- Placeholder values to be replaced

### 4. **Documentation** (`docs/`)
- **SETUP.md**: Step-by-step configuration guide
- **PROJECT_STRUCTURE.md**: This document
- **API_REFERENCE.md**: Firebase database schema
- **HARDWARE_GUIDE.md**: Wiring and hardware setup

## Firebase Database Schema

```
soilData/
  ├── moisture: integer (0-100)
  ├── status: string ("Dry" or "Wet")
  ├── pump: string ("ON" or "OFF")
  ├── timestamp: integer (Unix timestamp)
  └── manualPump: string ("ON" or "OFF") [Optional, for manual override]
```

## Pin Configuration (NodeMCU/ESP8266)

| Pin | Purpose | Connected To |
|-----|---------|--------------|
| D5  | Soil Moisture Sensor | Digital soil sensor |
| D6  | Relay Control | Water pump relay |
| GPIO2 | Built-in LED | Status indicator |

## Variable Names (DO NOT MODIFY)
These are preserved from the original code:
- `SOIL_PIN = D5`
- `RELAY_PIN = D6`
- `LED_PIN = 2`
- `UPDATE_INTERVAL = 10000` (milliseconds)

## Credential Management

### Arduino Credentials
1. **WiFi Credentials** → `hardware/wifi_config.h`
   - `WIFI_SSID`: Your network name
   - `WIFI_PASSWORD`: Your network password

2. **Firebase Credentials** → `hardware/firebase_config.h`
   - `FIREBASE_HOST`: Database URL
   - `FIREBASE_AUTH`: Database Secret

### Web Dashboard Credentials
1. **Firebase Config** → `web-dashboard/js/firebase-config.js`
   - Complete Firebase project configuration object
   - API keys, project ID, database URLs

### Security
- **All credential files are excluded from git** (see `.gitignore`)
- Never commit files containing real credentials
- Use example files as templates
- In production, consider using environment variables or secure backend services

## How to Use

### 1. Setup Arduino
1. Copy `hardware/wifi_config.h.example` to `hardware/wifi_config.h`
2. Copy `hardware/firebase_config.h.example` to `hardware/firebase_config.h`
3. Edit both files with your actual credentials
4. Upload `hardware/sensor_code.ino` to NodeMCU using Arduino IDE

### 2. Setup Web Dashboard
1. Copy `config/firebase-config.js.example` to `web-dashboard/js/firebase-config.js`
2. Edit with your Firebase project credentials
3. Open `web-dashboard/index.html` in a web browser
4. Dashboard will connect to Firebase and display real-time data

### 3. Monitor System
- View soil moisture percentage
- See pump status (ON/OFF)
- Check last update time
- View moisture trends on the chart
- Manually control pump with the "Water Plant" button

## Dependencies

### Arduino Libraries
- `ESP8266WiFi.h`: WiFi connectivity
- `ESP8266HTTPClient.h`: HTTP requests
- `ArduinoJson.h`: JSON serialization
- (Standard Arduino libraries)

### Web Dashboard
- **Firebase SDK v9**: Real-time database
  - Loaded from CDN (https://www.gstatic.com/firebasejs/9.22.1/)
- **Chart.js**: Data visualization
  - Loaded from CDN (https://cdn.jsdelivr.net/npm/chart.js)

## File Organization Best Practices

✅ **Good**:
- Separate concerns (hardware, web, config, docs)
- Configuration isolated from application code
- Credentials excluded from version control
- Clear naming conventions
- Comprehensive documentation

❌ **Avoid**:
- Hardcoded credentials in source files
- Mixing configuration with application logic
- Committing sensitive files
- Complex nested directory structures
- Undocumented code changes

## Git Workflow

```bash
# Clone the repository
git clone <repo-url>

# Create your local config files
cp config/wifi_config.h.example hardware/wifi_config.h
cp config/firebase_config.h.example hardware/firebase_config.h
cp config/firebase-config.js.example web-dashboard/js/firebase-config.js

# Edit with your credentials
# ... edit files ...

# Commit only non-credential files
git add .
git commit -m "Update project files"
git push

# The .gitignore will prevent credential files from being committed
```

## Troubleshooting

### Arduino Not Connecting to WiFi
- Check `wifi_config.h` for correct SSID and password
- Verify WiFi network is 2.4GHz (ESP8266 doesn't support 5GHz)
- Check Serial monitor output for error messages

### Dashboard Not Showing Data
- Verify `firebase-config.js` has correct credentials
- Check Firebase Realtime Database URL in console
- Ensure Arduino is connected and sending data
- Check browser console for errors

### Pump Not Responding
- Verify relay connection to D6 pin
- Check relay power supply
- Test manual override from dashboard
- Review Arduino serial output

## Academic Submission Notes

✅ **Well-organized** for viva explanation:
- Clear separation of concerns
- Professional documentation
- Logical folder hierarchy
- Simple, readable code
- No unnecessary complexity

✅ **Suitable for presentation**:
- Hardware module is self-contained
- Web dashboard is responsive and visually appealing
- Configuration management is straightforward
- Code follows best practices
- Documentation is comprehensive

## Support

For issues or questions:
1. Check the troubleshooting section
2. Review documentation in `docs/` folder
3. Check Arduino Serial Monitor output
4. Check browser console for JavaScript errors
5. Verify Firebase database connection
