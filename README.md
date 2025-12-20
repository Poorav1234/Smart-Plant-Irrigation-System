# IoT Based Smart Plant Irrigation and Monitoring System

A responsive dashboard for monitoring soil moisture, plant condition, and irrigation status in real-time using Firebase.

## Features

- Real-time monitoring of soil moisture percentage
- Visual indication of soil condition (Dry/Ideal/Wet)
- Pump status display (ON/OFF)
- Last updated timestamp
- Moisture level trend chart
- Responsive design that works on desktop and mobile devices
- Color-coded status indicators

## Setup Instructions

### 1. Firebase Configuration

1. Go to the [Firebase Console](https://console.firebase.google.com/)
2. Select your project "smart-plant-irrigation-s-d17fd"
3. Click the gear icon (Project Settings)
4. Scroll down to "Your apps" and copy the configuration object
5. Open `index.html` in a text editor
6. Find the `TODO` comment in the Firebase configuration section (around line 17)
7. Replace the placeholder values with your actual Firebase configuration:

```javascript
const firebaseConfig = {
  apiKey: "AIzaSyD**************************-***",
  authDomain: "smart-plant-irrigation-s-d17fd.firebaseapp.com",
  databaseURL: "https://smart-plant-irrigation-s-d17fd-default-rtdb.firebaseio.com",
  projectId: "smart-plant-irrigation-s-d17fd",
  storageBucket: "smart-plant-irrigation-s-d17fd.appspot.com",
  messagingSenderId: "123456789012",
  appId: "1:123456789012:web:abcdef1234567890abcdef"
};
```

### 2. Firebase Realtime Database Structure

Ensure your Firebase Realtime Database has the following structure:

```json
{
  "soilData": {
    "moisture": 55,
    "status": "Ideal",
    "pump": "OFF",
    "timestamp": 1700000000
  }
}
```

To set this up:
1. In your Firebase console, go to Realtime Database
2. Click "Create Database" if it doesn't exist
3. Choose "Start in test mode"
4. Click "Enable"
5. Add the data structure above by:
   - Clicking the "+" button next to the root
   - Enter "soilData" as the key
   - Add the child values as shown in the structure

### 3. Running the Dashboard

You can run the dashboard in two ways:

#### Option A: Direct Browser Opening (Simple)
1. Double-click the `index.html` file to open it in your browser
2. Note: Some browsers may block certain features when opening files directly

#### Option B: Local Server (Recommended)
If you have Python installed:
```bash
python -m http.server 8000
```
Then open http://localhost:8000 in your browser

If you have Node.js installed:
```bash
npm install -g http-server
http-server
```
Then open the provided URL in your browser

### 4. Testing Real-time Updates

1. Open the dashboard in your browser
2. Make sure you see the initial data from your Firebase database
3. In the Firebase console, change any value in the `soilData` object
4. Observe that the dashboard updates in real-time without refreshing

## Troubleshooting

### Common Issues

1. **Blank values or "--" displayed**: Check your Firebase configuration and security rules
2. **JavaScript errors in console**: Verify all configuration values are correct
3. **Chart not showing**: Ensure Chart.js library is loading properly (check network tab)
4. **Data not updating**: Check your Firebase security rules allow read access

### Checking for Errors

1. Press F12 in your browser to open Developer Tools
2. Go to the Console tab to see any error messages
3. Check the Network tab to verify Firebase connections are working

## Technologies Used

- HTML5
- CSS3 (with Flexbox and Grid layouts)
- JavaScript (ES6 modules)
- Firebase Realtime Database SDK v9
- Chart.js for data visualization

## Customization

You can customize the appearance by modifying the CSS in the `<style>` section of `index.html`:
- Colors: Modify the gradient in `body` and the color classes (.dry, .ideal, .wet)
- Card styles: Adjust padding, borders, and shadows in the `.card` class
- Typography: Change fonts and sizes in the relevant elements

## Support

For issues with this dashboard, please check:
1. All Firebase configuration values are correct
2. Your Firebase Realtime Database has the correct structure
3. Your Firebase security rules allow read access