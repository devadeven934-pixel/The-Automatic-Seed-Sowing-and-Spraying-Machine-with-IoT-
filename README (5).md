# 🌱 Agri-Bot IoT

**An autonomous, IoT-connected agricultural robot for seed sowing, ploughing, and spraying.**

Agri-Bot IoT is a 4-wheel-drive field robot built around an ESP32 microcontroller. It combines
mechanical actuation (seed metering, tilling, spraying) with sensor feedback (GPS, ultrasonic
obstacle detection, soil moisture) and remote cloud monitoring, so a farmer can automate
routine field tasks and track them from a dashboard.

![Agri-Bot IoT concept render](docs/images/agri-bot-visual.png)

---

## ✨ Features

- **IoT-based** — real-time telemetry and remote mode control over Wi-Fi (MQTT)
- **Multi-function** — plough, sow, and spray from a single platform
- **4-wheel drive** with differential steering (dual DC geared motors + L298N driver)
- **Automated seed sowing** via servo-driven metering plate and rotary-encoder distance tracking
- **Ploughing** via a linear-actuator-driven tilling blade with limit-switch depth control
- **Spraying** via 12V diaphragm pump, solenoid valve, and adjustable nozzle
- **Sensing** — NEO-6M GPS, HC-SR04 ultrasonic obstacle detection, capacitive soil moisture
- **Cloud dashboard** for remote monitoring and mode selection (plough / sow / spray / auto)

## 🧠 System Architecture

| Subsystem | Key Components |
|---|---|
| Controller | ESP32-WROOM-32D dev board (Wi-Fi + BLE, 34 GPIO, 12-bit ADC) |
| Power | 12V Li-ion battery pack → LM2596 buck converter (5V rail) |
| Drive | 2× 12V DC geared motors, L298N dual H-bridge driver, 4× wheels |
| Sowing | MG996R servo, seed hopper, metering plate, seed funnel |
| Ploughing | 12V linear actuator (100N/50mm), tilling blade, micro limit switch |
| Spraying | 12V diaphragm pump, relay module, solenoid valve, nozzle, tank |
| Sensing | NEO-6M GPS, HC-SR04 ultrasonic, capacitive soil moisture sensor, rotary encoder |
| Chassis | Aluminium base/side/front/rear panels, 3D-printed mounts |

**Signal flow:** Sensors/encoders → ESP32 (ADC/UART/GPIO) → decision logic → motor driver /
servo / relay outputs → mechanical actuators, with status published to the cloud over MQTT.

## 📦 Repository Structure

```
agri-bot-iot/
├── README.md                      ← you are here
├── docs/
│   ├── ASSEMBLY_GUIDE.md          ← fabrication, wiring, bring-up, assembly steps
│   └── images/
│       └── agri-bot-visual.png    ← concept render
├── hardware/
│   ├── BOM.csv                    ← full bill of materials with sourcing links
│   ├── electrical_connections.json← wire-by-wire power & signal connection list
│   ├── mechanical_connections.json← part-to-part mechanical attachment list
│   └── full_config.json           ← complete project spec (parts, specs, layout, pins)
└── firmware/                      ← PlatformIO ESP32 firmware (drive, sowing, ploughing,
    ├── platformio.ini                spraying, sensors, MQTT) — see firmware/README.md
    ├── include/
    └── src/
```

## 🛠️ Bill of Materials (summary)

55 line items in total — see [`hardware/BOM.csv`](hardware/BOM.csv) for full sourcing links and costs.
Major components:

| Component | Qty | Est. Cost |
|---|---|---|
| ESP32-WROOM-32D Dev Board | 1 | $7.50 |
| 12V DC Geared Motors (drive) | 2 | $30.00 |
| L298N Motor Driver | 1 | $6.00 |
| MG996R Servo (seed sowing) | 1 | $8.00 |
| 12V Linear Actuator (plough) | 1 | $25.00 |
| 12V Diaphragm Water Pump | 1 | $18.00 |
| 12V Solenoid Valve | 1 | $9.00 |
| NEO-6M GPS Module | 1 | $12.00 |
| HC-SR04 Ultrasonic Sensor | 1 | $2.50 |
| Capacitive Soil Moisture Sensor | 1 | $4.00 |
| 12V Li-ion Battery Pack | 1 | $40.00 |
| Chassis (aluminium panels + hardware) | — | ~$70.00 |

Estimated total build cost: **~$300–350** depending on sourcing.

## 🚀 Getting Started

### Tools required
3D printer (PETG/PLA), M3 hex key set, Phillips screwdriver, wire strippers, crimping tool,
multimeter, fine-tip soldering iron, heat gun (optional), utility/deburring knife.

### Assumptions
- Basic electronics assembly knowledge
- Familiarity with 3D printing
- Arduino IDE or PlatformIO with ESP32 board support installed
- 12V DC power supply for bench testing
- Basic understanding of ESP32 GPIO, PWM, UART, and analog inputs

### Build order
1. **Fabrication** — 3D print mounts, deburr parts, cut hose tubing
2. **Wiring** — power rails, sensor/actuator signal lines, continuity checks
3. **Bring-up** — flash test firmware, verify motors/servo/actuator/sensors individually
4. **Assembly** — chassis, drivetrain, sowing/ploughing/spraying subsystems, final wiring

Full step-by-step instructions: [`docs/ASSEMBLY_GUIDE.md`](docs/ASSEMBLY_GUIDE.md)

## 💻 Firmware

A working PlatformIO firmware is included in [`firmware/`](firmware/): differential drive,
servo-based seed sowing with encoder spacing, actuator-driven ploughing with limit-switch
stop, pump/solenoid spraying, ultrasonic obstacle stop, soil moisture + GPS sensing, and
MQTT telemetry/remote mode control. See [`firmware/README.md`](firmware/README.md) for
setup, the pin map, and MQTT command/telemetry formats — including a flagged assumption
about actuator/solenoid wiring not specified in the original hardware plan.

## 🗺️ Roadmap

- [x] Firmware: sensor drivers (GPS, ultrasonic, soil moisture, encoder)
- [x] Firmware: motor/servo/actuator control logic
- [x] MQTT integration for cloud dashboard telemetry and mode commands
- [ ] `auto` mode: full field-coverage logic (waypoints, headland turns, combined plough+sow+spray)
- [ ] Autonomous navigation / obstacle avoidance beyond simple stop-on-detect
- [ ] Field calibration (seed rate, plough depth, spray volume)
- [ ] OTA firmware updates

## 🤝 Contributing

This is an open hardware/software project — contributions, part substitutions, and firmware
implementations are welcome. Open an issue or pull request.

## 📄 License

Add a license of your choice (MIT is common for open hardware projects) as `LICENSE` in the repo root.
