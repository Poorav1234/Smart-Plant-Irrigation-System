# Quick Start Guide

## After Cloning/Downloading the Project

### Step 1: Copy Configuration Templates (2 minutes)

```bash
# Navigate to project directory
cd Smart-Plant-Irrigation-System

# Copy Arduino configs
cp config/wifi_config.h.example hardware/wifi_config.h
cp config/firebase_config.h.example hardware/firebase_config.h

# Copy web config
cp config/firebase-config.js.example web-dashboard/js/firebase-config.js
```

### Step 2: Fill in Your Credentials

#### WiFi Config (`hardware/wifi_config.h`)
```cpp
#define WIFI_SSID "YOUR_WIFI_NAME"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

#### Firebase Arduino (`hardware/firebase_config.h`)
```cpp
#define FIREBASE_HOST "your-project-rtdb.firebaseio.com"
#define FIREBASE_AUTH "your-auth-token"
```

#### Firebase Web (`web-dashboard/js/firebase-config.js`)
```javascript
const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "your-project.firebaseapp.com",
    databaseURL: "https://your-project-rtdb.firebaseio.com",
    projectId: "your-project",
    storageBucket: "your-project.appspot.com",
    messagingSenderId: "your-messaging-id",
    appId: "your-app-id",
    measurementId: "your-measurement-id"
};
```

### Step 3: Upload Arduino Code

1. Open Arduino IDE
2. Open `hardware/sensor_code.ino`
3. Connect NodeMCU via USB
4. Select **Tools → Board → NodeMCU 1.0 (ESP-12E)**
5. Select **Tools → Port → COM[X]**
6. Click **Upload** (→ button)
7. Wait for "Done uploading" message

### Step 4: Open Web Dashboard

1. Open `web-dashboard/index.html` in a web browser
2. Dashboard should connect to Firebase automatically
3. Wait for data from Arduino (10-second updates)
4. You should see:
   - Real-time sensor readings
   - Soil condition status
   - Pump status
   - Moisture trend chart

### Step 5: Test the System

**Automatic Mode**:
- Keep soil dry → Pump turns ON
- Add moisture → Pump turns OFF

**Manual Mode**:
- Click "Water Plant" button → Pump turns ON
- Click "Stop Watering" → Pump turns OFF

---

## Project Structure

```
Smart-Plant-Irrigation-System/
├── hardware/                    # Arduino code
│   ├── sensor_code.ino         # Main code
│   ├── wifi_config.h           # WiFi credentials (CREATE THIS)
│   └── firebase_config.h       # Firebase creds (CREATE THIS)
│
├── web-dashboard/              # Web frontend
│   ├── index.html              # HTML page
│   ├── css/styles.css          # Styling
│   └── js/
│       ├── app.js              # Application logic
│       └── firebase-config.js  # Firebase config (CREATE THIS)
│
├── config/                      # Templates
│   ├── wifi_config.h.example
│   ├── firebase_config.h.example
│   └── firebase-config.js.example
│
├── docs/                        # Documentation
│   ├── SETUP.md                # Full setup guide
│   ├── HARDWARE_GUIDE.md       # Wiring & pinout
│   ├── API_REFERENCE.md        # Database schema
│   └── README files
│
├── .gitignore                   # Git security
└── README.md
```

---

## Getting Firebase Credentials

### Firebase Host & Auth Token (for Arduino)

1. Go to https://console.firebase.google.com/
2. Select your project
3. Click **Realtime Database**
4. Copy the database URL (the part between `https://` and `/`)
5. For auth token:
   - Click **Rules** tab
   - You'll see your **Database Secret** in the docs
   - Copy that value

### Firebase Config (for Web)

1. Go to https://console.firebase.google.com/
2. Select your project
3. Click **Settings** (⚙️) → **Project Settings**
4. Scroll to **Your apps** section
5. Click your web app (or create one)
6. Copy the entire `firebaseConfig` object
7. Paste into `web-dashboard/js/firebase-config.js`

---

## File Checklist

Before you're ready to go:

- [ ] `hardware/wifi_config.h` created and filled
- [ ] `hardware/firebase_config.h` created and filled
- [ ] `web-dashboard/js/firebase-config.js` created and filled
- [ ] Arduino code uploaded to NodeMCU
- [ ] Web dashboard opens in browser
- [ ] Dashboard connects to Firebase (no errors in console)
- [ ] Arduino sends data (check Serial Monitor)
- [ ] Dashboard displays real-time data
- [ ] Manual pump control works

---

## Troubleshooting

### Arduino Won't Connect to WiFi
- ✓ Check SSID and password in `wifi_config.h`
- ✓ Ensure WiFi is 2.4GHz (not 5GHz)
- ✓ Check Arduino Serial Monitor for error messages

### Dashboard Shows "--" for All Values
- ✓ Check `firebase-config.js` values
- ✓ Verify Arduino is connected and sending data
- ✓ Open browser console (F12) for errors

### Manual Button Doesn't Work
- ✓ Check browser console for errors
- ✓ Verify Firebase security rules allow write access
- ✓ Check network connection

### Firebase Connection Error
- ✓ Verify API key and database URL are correct
- ✓ Check Firebase console for data updates
- ✓ Ensure database rules allow read/write

---

## Documentation

For detailed information, see:

| Doc | Topic |
|-----|-------|
| `SETUP.md` | Step-by-step configuration |
| `HARDWARE_GUIDE.md` | Pin connections and wiring |
| `API_REFERENCE.md` | Database schema and data flow |
| `PROJECT_STRUCTURE.md` | Project organization |

---

## Security Notes

⚠️ **Important**:
- Your credential files are NOT included in git (see `.gitignore`)
- Never commit `wifi_config.h`, `firebase_config.h`, or `firebase-config.js`
- These files contain sensitive information
- Safe to commit everything else

---

## Next Steps

1. ✅ Copy configuration templates
2. ✅ Fill in your credentials
3. ✅ Upload Arduino code
4. ✅ Open web dashboard
5. ✅ Test all features
6. ✅ Review documentation
7. ✅ Prepare for presentation

---

## Key Variables (DO NOT MODIFY)

These are preserved from the original code:
- `SOIL_PIN = D5`
- `RELAY_PIN = D6`
- `LED_PIN = 2`
- `UPDATE_INTERVAL = 10000` ms

All sensor logic and database keys are unchanged.

---

## Support

For detailed help:
1. Check `docs/` folder for comprehensive guides
2. Review inline code comments
3. Check Arduino Serial Monitor output
4. Check browser console (F12) for web errors

---

**Ready to go!** Follow the steps above to get your Smart Irrigation System up and running.
