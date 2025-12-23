# Hardware Setup and Pin Configuration

## Hardware Components

### Required Components
1. **Microcontroller**: NodeMCU v3 (ESP8266-12E)
2. **Soil Moisture Sensor**: Digital capacitive soil sensor with output pin
3. **Relay Module**: 5V single-channel relay module
4. **Water Pump**: 5V-12V DC water pump
5. **Power Supply**: 5V/2A USB adapter for NodeMCU, appropriate PSU for pump
6. **Connecting Components**:
   - Breadboard
   - Jumper wires (male-to-male and male-to-female)
   - USB Micro-B cable for programming

### Optional Components
- Resistors (10kΩ for protection)
- Capacitors (100µF for stability)
- Diode (1N4007 for relay protection)

## Pin Configuration

### NodeMCU Pinout
```
NodeMCU (ESP8266-12E)
┌────────────────────────────────┐
│ GND  D0   D1   D2   D3   D4    │
│ 3V3  D5   D6   D7   D8   RX    │
│ GND  TX   SDA  SCL  SD3  SD2   │
│ VIN  RST  5V   GND  D9   D10   │
└────────────────────────────────┘
```

### Connected Pins (DO NOT MODIFY)

| NodeMCU Pin | Pin Label | Purpose | Connected To |
|-------------|-----------|---------|--------------|
| D5 | GPIO14 | Soil Sensor Input | Digital Moisture Sensor |
| D6 | GPIO12 | Relay Control Output | Relay Module Signal |
| GPIO2 | GPIO2 | Built-in LED | Status Indicator |
| GND | GND | Ground | Common ground |
| 3V3 | 3V3 | 3.3V Power | Logic power for sensors |
| VIN | VIN | Input Power | 5V USB adapter |

### Circuit Diagram

```
NodeMCU (ESP8266)              Sensors & Actuators
┌─────────────────┐
│                 │
│  D5 (GPIO14)────────→ Soil Moisture Sensor (Digital Output)
│                 │
│  D6 (GPIO12)────────→ Relay Module (Signal Pin)
│                 │
│  GPIO2──────────────→ Built-in LED (via resistor)
│                 │
│  3V3 ─────────────→ Sensor Power (3.3V line)
│                 │
│  GND ─────────────→ Common Ground
│                 │
│  VIN ◄─ 5V USB Power
│                 │
└─────────────────┘
```

## Connection Details

### 1. Soil Moisture Sensor Connection

**Sensor Pins**:
- **VCC**: 3.3V (from NodeMCU 3V3)
- **GND**: Ground (from NodeMCU GND)
- **DO** (Digital Output): D5 (GPIO14)
- **AO** (Analog Output): Not used in this project

**Wiring**:
```
Soil Sensor        NodeMCU
   VCC    ────────→ 3V3
   GND    ────────→ GND
   DO     ────────→ D5
```

**Sensor Logic**:
- **HIGH (1)**: Dry soil (no moisture detected)
- **LOW (0)**: Wet soil (moisture detected)

### 2. Relay Module Connection

**Relay Module Pins**:
- **VCC**: 5V power
- **GND**: Ground
- **IN** (Signal Input): D6 (GPIO12)

**Relay Output** (Pump Control):
- **NO** (Normally Open): To pump positive wire
- **COM** (Common): To power supply positive
- **NC** (Normally Closed): Not used

**Wiring**:
```
Relay Module       NodeMCU          Power Supply
   VCC    ─────────→ VIN (5V) or external 5V
   GND    ─────────→ GND
   IN     ─────────→ D6

Pump Control (Relay Contacts):
   COM    ─────────→ 5V+ (Power Supply)
   NO     ─────────→ Pump Positive Wire
   Pump GND ──────→ Power Supply Ground
```

**Relay Logic**:
- **D6 = LOW (0)**: Relay activated → Pump ON
- **D6 = HIGH (1)**: Relay deactivated → Pump OFF

### 3. Built-in LED Connection

**LED Pin**: GPIO2 (integrated on NodeMCU)

**LED Logic**:
- **GPIO2 = LOW (0)**: LED ON (Pump active)
- **GPIO2 = HIGH (1)**: LED OFF (Pump inactive)

## Complete Schematic

```
┌────────────────────────────────────────────────────────────┐
│                    SMART IRRIGATION CIRCUIT                 │
└────────────────────────────────────────────────────────────┘

                        NodeMCU (ESP8266)
                    ┌──────────────────────┐
                    │                      │
    Soil Moisture ──→ D5/GPIO14            │
    Sensor       │                      │
                    │   3V3 ──┬──────────→ Sensor Power
                    │         │           │
                    │   GND ──┼──────────→ Sensor Ground
                    │         │           │
                    │         │   VCC ──→ Relay VCC
                    │   D6/GPIO12        │
    Relay Module ───→          │         │
    Signal Input     │   GND ──┼──────────→ Relay Ground
                    │         │           │
                    │   GPIO2              │ (Built-in LED)
                    │                      │
                    │   VIN ◄─── 5V USB Power
                    │                      │
                    └──────────────────────┘
                           │
                           │ GND
                           ▼
                    ┌──────────────┐
                    │   5V Power   │
                    │   Supply     │
                    │              │
    Pump GND ◄──────┤ GND          │
    Pump +5V ◄──────┤ +5V (via NO) │
                    └──────────────┘
                           ▲
                           │
                         Relay Contact (NO)
```

## Pin Constraints

### Do NOT Use These Pins
- **D0**: Reset pin (may cause issues)
- **D4**: Interferes with SPIFFS
- **D8**: Boot issues
- **RX/TX**: Serial communication (for debugging)
- **A0**: Analog input (not used for sensors in this project)

### Safe Pins for Future Expansion
- GPIO1 (RX) - Only if serial debug not needed
- GPIO3 (TX) - Only if serial debug not needed
- GPIO15, GPIO0, GPIO2 - With constraints
- A0 - Analog input (for future analog sensor)

## Assembly Instructions

### Step 1: Prepare the Breadboard
1. Place NodeMCU on breadboard
2. Identify D5, D6, 3V3, GND, and VIN pins
3. Use breadboard power rails for 3V3 and GND distribution

### Step 2: Connect Soil Moisture Sensor
1. Connect sensor VCC to 3V3 rail
2. Connect sensor GND to GND rail
3. Connect sensor DO to D5 pin
4. Test with Serial Monitor (should show HIGH when dry, LOW when wet)

### Step 3: Connect Relay Module
1. Connect relay VCC to VIN (5V)
2. Connect relay GND to GND rail
3. Connect relay IN (signal) to D6 pin

### Step 4: Connect Water Pump
1. Connect 5V power supply positive to relay COM
2. Connect pump positive to relay NO
3. Connect pump negative to power supply negative
4. Use proper connectors (not breadboard for power)

### Step 5: Test Connections
1. Upload Arduino code
2. Check Serial Monitor for sensor readings
3. Watch for relay click when D6 changes state
4. Verify LED blinks with pump activity

## Testing

### Dry Soil Test
1. Keep soil sensor dry
2. Observe Serial Monitor:
   ```
   Soil Sensor Value: 1
   Soil is DRY - Activating pump
   ```
3. Relay should click and activate
4. Pump should run
5. LED should turn on

### Wet Soil Test
1. Apply moisture to sensor
2. Observe Serial Monitor:
   ```
   Soil Sensor Value: 0
   Soil is WET - Deactivating pump
   ```
3. Relay should click and deactivate
4. Pump should stop
5. LED should turn off

### Manual Override Test
1. From web dashboard, click "Water Plant"
2. Relay should activate regardless of soil state
3. Serial Monitor should show:
   ```
   Manual pump override: ON
   Pump turned ON by manual override
   ```

## Troubleshooting Hardware

| Issue | Cause | Solution |
|-------|-------|----------|
| Sensor always reads DRY | Incorrect pin connection | Verify D5 connection |
| Relay doesn't click | Low GPIO signal | Check D6 pin, may need current limiting resistor |
| Pump doesn't run | Relay not switching properly | Test relay with multimeter, check power supply |
| LED doesn't light | GPIO2 not connected | Verify LED is functional, check pin definition |
| NodeMCU reboots | Power issue | Use proper 5V PSU, not USB hub; add capacitor |
| Sensor reading flickering | Loose connection | Reseat all jumper wires, use shorter jumpers |

## Power Considerations

### Power Supply Requirements
- **NodeMCU + Sensors**: 500mA @ 5V
- **Relay**: 70mA @ 5V
- **Water Pump**: 500mA-2A @ 5V-12V (check pump specs)
- **Total**: 1A-3A @ 5V minimum

### Recommended Power Supply
- **Type**: Regulated DC power supply
- **Voltage**: 5V
- **Current**: 3A or higher
- **Connector**: USB or barrel connector

### Power Stability
- Use separate power supply for pump (high current)
- Add 100µF capacitor across pump power
- Add 10µF capacitor across relay coil
- Keep USB cable short and quality for NodeMCU

## Safety Notes

⚠️ **Important**:
- Use appropriate power supply for pump (not USB alone)
- Use relay to isolate high current from NodeMCU
- Add diode across relay coil for protection
- Ensure proper water isolation (no exposed circuits)
- Test all connections before powering on pump
- Use proper wire gauges for high current paths

## Future Expansion

### Possible Pin Uses for Additional Sensors
- **Analog Input (A0)**: Temperature or humidity sensor
- **D0/D1**: I2C communication (SDA/SCL available)
- **D7/D8**: Additional digital sensors
- **D3/D4**: With caution (boot/SPIFFS related)

### Hardware Upgrades
- Replace digital sensor with analog for finer control
- Add DHT11 for temperature/humidity
- Add water level sensor
- Add multiple pump zones with additional relays
