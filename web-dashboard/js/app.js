/**
 * Smart Plant Irrigation System - Web Dashboard
 * Main Application Logic
 * 
 * This module initializes Firebase, sets up real-time data listeners,
 * and handles UI updates and user interactions.
 */

import { initializeApp } from "https://www.gstatic.com/firebasejs/9.22.1/firebase-app.js";
import { getDatabase, ref, onValue, set } from "https://www.gstatic.com/firebasejs/9.22.1/firebase-database.js";
import { firebaseConfig } from "./firebase-config.js";

// Initialize Firebase (ONLY ONCE)
const app = initializeApp(firebaseConfig);

// Get database reference
const database = getDatabase(app);

// Get references to HTML elements
const moistureElement = document.getElementById('moisture');
const statusElement = document.getElementById('status');
const pumpElement = document.getElementById('pump');
const timestampElement = document.getElementById('timestamp');
const statusCard = document.getElementById('status-card');
const waterButton = document.getElementById('water-button');

// Initialize the moisture level chart
let moistureChart;

/**
 * Initialize the moisture trend chart using Chart.js
 */
function initializeMoistureChart() {
    const ctx = document.getElementById('moistureChart').getContext('2d');
    moistureChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Soil Moisture (%)',
                data: [],
                borderColor: 'rgb(75, 192, 192)',
                backgroundColor: 'rgba(75, 192, 192, 0.2)',
                tension: 0.1
            }]
        },
        options: {
            responsive: true,
            scales: {
                y: {
                    beginAtZero: true,
                    max: 100
                }
            }
        }
    });
}

/**
 * Set up real-time listener for Firebase database changes
 */
function setupRealtimeListener() {
    const soilDataRef = ref(database, 'soilData');

    onValue(soilDataRef, (snapshot) => {
        const data = snapshot.val();

        if (!data) return;

        moistureElement.textContent = data.moisture;
        statusElement.textContent = data.status;
        pumpElement.textContent = data.pump;

        const date = new Date(data.timestamp * 1000);
        timestampElement.textContent = date.toLocaleString();

        statusCard.className = 'card';
        if (data.status === 'Dry') statusCard.classList.add('dry');
        if (data.status === 'Wet') statusCard.classList.add('wet');

        if (data.pump === 'ON') {
            waterButton.textContent = 'Stop Watering';
            waterButton.classList.add('watering');
        } else {
            waterButton.textContent = 'Water Plant';
            waterButton.classList.remove('watering');
        }

        const now = new Date();
        moistureChart.data.labels.push(now.toLocaleTimeString());
        moistureChart.data.datasets[0].data.push(data.moisture);

        if (moistureChart.data.labels.length > 10) {
            moistureChart.data.labels.shift();
            moistureChart.data.datasets[0].data.shift();
        }

        moistureChart.update();
    });
}

/**
 * Handle manual watering button click
 */
function setupWaterButtonListener() {
    waterButton.addEventListener('click', () => {
        const currentPumpStatus = pumpElement.textContent;
        const newPumpStatus = (currentPumpStatus === 'ON') ? 'OFF' : 'ON';

        set(ref(database, 'soilData/manualPump'), newPumpStatus)
            .then(() => {
                console.log('Manual pump override set to:', newPumpStatus);
            })
            .catch((error) => {
                console.error('Error updating pump status:', error);
            });
    });
}

/**
 * Initialize Dashboard (RENAMED — avoids Firebase conflict)
 */
function initializeDashboard() {
    console.log('Initializing Smart Plant Irrigation System Dashboard...');

    initializeMoistureChart();
    setupRealtimeListener();
    setupWaterButtonListener();

    console.log('Dashboard initialized successfully!');
}

// Run after DOM is ready
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', initializeDashboard);
} else {
    initializeDashboard();
}
