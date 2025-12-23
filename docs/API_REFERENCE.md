# Firebase Database API Reference

## Database Overview

The Smart Irrigation System uses Firebase Realtime Database for storing and retrieving sensor data and pump control commands.

## Database Structure

```
root/
└── soilData/
    ├── moisture (integer)
    ├── status (string)
    ├── pump (string)
    ├── timestamp (integer)
    └── manualPump (string) [Optional]
```

## Data Paths and Field Descriptions

### 1. `/soilData/moisture`

**Type**: Integer (0-100)

**Description**: Current soil moisture level as a percentage.

**Values**:
- `0`: Dry soil (no moisture detected)
- `100`: Wet soil (moisture detected)
- `50`: Medium moisture (for future analog sensor use)

**Updated by**: Arduino sensor code every 10 seconds

**Read by**: Web dashboard, mobile apps

**Example**:
```json
{
  "moisture": 0
}
```

---

### 2. `/soilData/status`

**Type**: String

**Description**: Human-readable soil moisture status.

**Possible Values**:
- `"Dry"`: Soil is dry, pump should be active
- `"Wet"`: Soil is wet, pump should be inactive
- `"Ideal"`: (Future) Optimal moisture level

**Updated by**: Arduino sensor code every 10 seconds

**Read by**: Web dashboard for UI display and status card coloring

**Example**:
```json
{
  "status": "Dry"
}
```

**UI Behavior**:
- **Dry** → Red background color (#ff6464)
- **Wet** → Blue background color (#6496ff)
- **Ideal** → Green background color (#64ff64)

---

### 3. `/soilData/pump`

**Type**: String

**Description**: Current pump operating status.

**Possible Values**:
- `"ON"`: Pump is actively watering the plant
- `"OFF"`: Pump is not running

**Updated by**: Arduino sensor code every 10 seconds

**Read by**: Web dashboard for status display and button state

**Example**:
```json
{
  "pump": "ON"
}
```

**Hardware Control**:
- **D6 = LOW (0)** → Pump "ON"
- **D6 = HIGH (1)** → Pump "OFF"

---

### 4. `/soilData/timestamp`

**Type**: Integer (Unix timestamp in seconds)

**Description**: When the sensor data was last updated.

**Format**: Unix timestamp (seconds since Jan 1, 1970 UTC)

**Updated by**: Arduino sensor code every 10 seconds

**Read by**: Web dashboard for "Last Updated" display

**Example**:
```json
{
  "timestamp": 1703347200
}
```

**Web Conversion**:
```javascript
const date = new Date(data.timestamp * 1000);  // Convert to milliseconds
const readable = date.toLocaleString();        // "12/23/2024, 10:30:45 AM"
```

---

### 5. `/soilData/manualPump` [Optional]

**Type**: String

**Description**: Manual pump override command from web dashboard.

**Possible Values**:
- `"ON"`: Force pump to turn on (override auto mode)
- `"OFF"`: Force pump to turn off (override auto mode)
- `null` or absent: No manual override (use automatic control)

**Updated by**: Web dashboard when user clicks "Water Plant" button

**Read by**: Arduino code every 10 seconds

**Example**:
```json
{
  "manualPump": "ON"
}
```

**Arduino Logic**:
```cpp
if (response == "ON") {
    digitalWrite(RELAY_PIN, LOW);   // Activate pump
    Serial.println("Pump turned ON by manual override");
}
else if (response == "OFF") {
    digitalWrite(RELAY_PIN, HIGH);  // Deactivate pump
    Serial.println("Pump turned OFF by manual override");
}
```

## Complete Database State Example

```json
{
  "soilData": {
    "moisture": 0,
    "status": "Dry",
    "pump": "ON",
    "timestamp": 1703347245,
    "manualPump": null
  }
}
```

## Firebase Rules

### Security Rules for Realtime Database

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

⚠️ **Note**: This allows public read/write access. For production, implement proper authentication:

```json
{
  "rules": {
    "soilData": {
      ".read": "auth != null",
      ".write": "auth != null"
    }
  }
}
```

## API Operations

### Write Operations (Arduino)

#### Update Sensor Data
**Path**: `/soilData.json`

**Method**: PATCH (HTTP)

**Payload**:
```json
{
  "moisture": 0,
  "status": "Dry",
  "pump": "ON",
  "timestamp": 1703347245
}
```

**Arduino Code**:
```cpp
String url = "https://" + String(FIREBASE_HOST) + "/soilData.json?auth=" + String(FIREBASE_AUTH);
http.begin(client, url);
http.addHeader("Content-Type", "application/json");
int httpCode = http.PATCH(jsonString);
```

---

#### Check Manual Override
**Path**: `/soilData/manualPump.json`

**Method**: GET (HTTP)

**Response**:
```
"ON"    (with quotes)
"OFF"   (with quotes)
null    (if not set)
```

**Arduino Code**:
```cpp
String url = "https://" + String(FIREBASE_HOST) + "/soilData/manualPump.json?auth=" + String(FIREBASE_AUTH);
http.begin(client, url);
int httpCode = http.GET();
String response = http.getString();
```

---

### Write Operations (Web Dashboard)

#### Set Manual Override
**Path**: `/soilData/manualPump`

**Method**: set() (Firebase SDK v9)

**Payload**:
```json
"ON"    or    "OFF"
```

**JavaScript Code**:
```javascript
import { set, ref } from "firebase/database";

set(ref(database, 'soilData/manualPump'), 'ON')
    .then(() => console.log('Pump override set to ON'))
    .catch((error) => console.error('Error:', error));
```

---

### Read Operations (Web Dashboard)

#### Listen for Real-time Updates
**Path**: `/soilData`

**Method**: onValue() (Firebase SDK v9)

**Response Structure**:
```json
{
  "moisture": 0,
  "status": "Dry",
  "pump": "ON",
  "timestamp": 1703347245
}
```

**JavaScript Code**:
```javascript
import { onValue, ref } from "firebase/database";

const soilDataRef = ref(database, 'soilData');
onValue(soilDataRef, (snapshot) => {
    const data = snapshot.val();
    console.log('Moisture:', data.moisture);
    console.log('Status:', data.status);
    console.log('Pump:', data.pump);
    console.log('Updated at:', new Date(data.timestamp * 1000));
});
```

## Data Update Cycle

### Arduino Update Sequence (Every 10 seconds)

```
1. Read soil sensor (D5)
   ↓
2. Determine soil status (Dry/Wet)
   ↓
3. Check manual override (/soilData/manualPump)
   ↓
4. Control pump (D6)
   ↓
5. Prepare JSON payload
   ↓
6. Send data to Firebase (/soilData)
   ↓
7. Wait 10 seconds
```

### Web Dashboard Update Sequence (Real-time)

```
1. Firebase detects change at /soilData
   ↓
2. onValue listener triggers
   ↓
3. Update UI cards (moisture, status, pump, timestamp)
   ↓
4. Update status card color
   ↓
5. Add data point to chart
   ↓
6. Ready for next update
```

### Manual Pump Control Sequence

```
User Action (Web Dashboard):
1. Click "Water Plant" button
   ↓
2. Read current pump status
   ↓
3. Determine new status (ON ↔ OFF)
   ↓
4. Write to /soilData/manualPump
   ↓
5. Update button UI

Arduino Response:
1. Read /soilData/manualPump every 10 seconds
   ↓
2. If override found:
   - Control pump accordingly
   - Update /soilData/pump status
   ↓
3. If no override:
   - Use automatic control based on sensor
```

## JSON Payloads

### Arduino Read Response (GET manualPump)

**When set to ON**:
```
"ON"
```

**When set to OFF**:
```
"OFF"
```

**When not set**:
```
null
```

---

### Arduino Write Payload (PATCH soilData)

```json
{
  "moisture": 0,
  "status": "Dry",
  "pump": "ON",
  "timestamp": 1703347245
}
```

---

### Firebase Snapshot (onValue)

```json
{
  "moisture": 0,
  "status": "Dry",
  "pump": "ON",
  "timestamp": 1703347245
}
```

## Error Handling

### Arduino HTTP Errors

| HTTP Code | Meaning | Solution |
|-----------|---------|----------|
| 200 | OK | Data sent successfully |
| 401 | Unauthorized | Check FIREBASE_AUTH token |
| 403 | Forbidden | Check Firebase security rules |
| 404 | Not Found | Check FIREBASE_HOST URL |
| Connection timeout | Network issue | Check WiFi connection |

### Web Dashboard Errors

| Error | Cause | Solution |
|-------|-------|----------|
| "firebaseConfig is not defined" | Missing firebase-config.js | Create and configure firebase-config.js |
| "Cannot read property 'moisture'" | No data in database | Wait for Arduino to send data |
| Manual override not working | Security rules too strict | Check Firebase rules allow write access |
| Chart not updating | onValue listener not working | Check database path and config |

## Testing Data

### Test Write (Via Arduino Serial)

```cpp
// Manually test data format
String testData = "{\"moisture\":50,\"status\":\"Ideal\",\"pump\":\"OFF\",\"timestamp\":1703347245}";
// Send to Firebase
```

### Test Read (Via Firebase Console)

1. Go to Firebase Console
2. Click **Database** → **Data** tab
3. Manually edit `/soilData` values
4. Watch web dashboard update in real-time

### Test Manual Override (Via Firebase Console)

1. Go to Firebase Console
2. Click **Database** → **Data** tab
3. Add/modify `/soilData/manualPump` to "ON" or "OFF"
4. Watch Arduino respond in Serial Monitor

## Optimization Tips

### Reduce Data Payload
Current payload size: ~80 bytes per update
- Consider only sending changed fields
- Use shorter field names in production
- Implement delta updates

### Reduce Update Frequency
Current frequency: Every 10 seconds
- Increase `UPDATE_INTERVAL` if data changes are slow
- Implement change-triggered updates (only send when value changes)

### Bandwidth Calculation
- 10 seconds update interval
- 80 bytes per update
- = 8 bytes per second
- = 690 KB per day
- = 20 MB per month (acceptable for free Firebase tier)

## Real-time Database Limitations

- **Simultaneous connections**: 100 (free tier)
- **Storage**: 1 GB (free tier)
- **Transactions**: Limited to single path
- **Query depth**: Should not exceed 25 levels

For this project, we're well within free tier limits.

## Related Files

- Arduino implementation: `hardware/sensor_code.ino`
- Web implementation: `web-dashboard/js/app.js`
- Web config: `web-dashboard/js/firebase-config.js`
- Arduino config: `hardware/firebase_config.h`
