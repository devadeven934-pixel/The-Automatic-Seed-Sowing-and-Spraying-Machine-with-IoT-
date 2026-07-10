# Firmware

PlatformIO project for the ESP32-WROOM-32D controller. Implements drive control, seed
sowing, ploughing, spraying, sensor reads (GPS/ultrasonic/soil moisture/encoder), and
MQTT-based remote monitoring/control.

## Structure

```
firmware/
├── platformio.ini
├── include/
│   ├── config.h        # pin map, Wi-Fi/MQTT credentials, tunable constants
│   ├── drive.h
│   ├── sowing.h
│   ├── ploughing.h
│   ├── spraying.h
│   ├── sensors.h
│   └── net.h
└── src/
    ├── main.cpp         # mode state machine, MQTT command dispatch, telemetry
    ├── drive.cpp         # differential drive via L298N (PWM + direction pins)
    ├── sowing.cpp        # servo seed metering, wheel-encoder-based spacing
    ├── ploughing.cpp     # linear actuator lower/raise, limit-switch stop
    ├── spraying.cpp      # pump relay + solenoid valve
    ├── sensors.cpp       # HC-SR04 ultrasonic, capacitive soil moisture, NEO-6M GPS
    └── net.cpp           # Wi-Fi + MQTT connect/reconnect, publish/subscribe
```

## Setup

1. Install [PlatformIO](https://platformio.org/) (VS Code extension or CLI).
2. Edit `include/config.h`:
   - Set `WIFI_SSID` / `WIFI_PASSWORD`
   - Set `MQTT_BROKER` (and `MQTT_USER`/`MQTT_PASSWORD` if your broker needs auth)
   - Review the pin map — see the **assumption notice** at the top of the file
3. Build & upload:
   ```bash
   cd firmware
   pio run -t upload
   pio device monitor
   ```

## ⚠️ Pin map assumption

`hardware/electrical_connections.json` documents a **single relay for the water pump**
only. It does not specify a control line for the plough linear actuator's direction or
for a spraying solenoid valve. `config.h` assumes:

- A 2-channel relay (or small H-bridge) drives the linear actuator forward/reverse on
  `PLOUGH_EXTEND_PIN` / `PLOUGH_RETRACT_PIN`.
- A second relay/MOSFET drives the solenoid valve on `SOLENOID_PIN`.

If your actual wiring differs (e.g. actuator wired directly to battery with no ESP32
control, or solenoid sharing the pump relay), adjust `ploughing.cpp` / `spraying.cpp`
and the pin definitions accordingly.

## Remote control (MQTT)

**Command topic:** `agribot/agribot-01/command` — publish JSON:

```json
{"mode": "sow"}
{"mode": "plough"}
{"mode": "spray"}
{"mode": "manual", "left": 150, "right": 150}
{"mode": "idle"}
```

**Telemetry topic:** `agribot/agribot-01/telemetry` — published every 2s (JSON), e.g.:

```json
{
  "mode": "sow",
  "soil_moisture_pct": 42,
  "obstacle_cm": 87.3,
  "distance_travelled_cm": 154.2,
  "gps_fix": true,
  "lat": 12.9716,
  "lng": 77.5946,
  "satellites": 7,
  "plough_limit_reached": false,
  "spraying": false,
  "mqtt_connected": true
}
```

Change the client ID / topics in `config.h` if you run multiple bots.

## What's implemented vs. left as TODO

| Area | Status |
|---|---|
| Differential drive (PWM + direction) | Done |
| Seed sowing (servo + encoder spacing) | Done |
| Ploughing (actuator + limit switch, timed safety cutoff) | Done |
| Spraying (pump + solenoid) | Done |
| Ultrasonic obstacle stop | Done |
| Soil moisture read | Done — calibrate `SOIL_ADC_DRY`/`SOIL_ADC_WET` for your sensor |
| GPS parsing | Done |
| Wi-Fi/MQTT connect, reconnect, telemetry, commands | Done |
| `auto` mode — full field-coverage logic (waypoints, headland turns, combined plough+sow+spray sequencing) | Placeholder only — see `TODO` comment in `main.cpp` |
| OTA updates | Not implemented |
| Persistent config (Wi-Fi/MQTT via web portal instead of hardcoded) | Not implemented |

Contributions filling in the `auto` mode logic or adding OTA/config-portal support are welcome.
