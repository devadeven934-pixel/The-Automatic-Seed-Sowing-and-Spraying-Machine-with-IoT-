# Assembly Guide

## Tools
- 3D printer (PETG/PLA capable)
- M3 hex key set
- Small Phillips screwdriver
- Wire strippers
- Crimping tool (optional, for custom wires)
- Multimeter
- Soldering iron with fine tip (for ESP32 headers, if not pre-soldered)
- Heat gun (for heat shrink, if used)
- Utility knife / deburring tool

## Assumptions
- Basic electronics assembly knowledge
- Familiarity with 3D printing processes
- Arduino IDE or PlatformIO installed with ESP32 board support
- 12V DC power supply for initial testing
- Basic understanding of ESP32 GPIO, PWM, UART, and analog inputs

---

## 1. Fabrication
1.1 3D print all required mounts and custom parts
1.2 Deburr and clean all 3D printed parts and aluminium panels
1.3 Cut and prepare water hose tubing to length for connections
1.4 Test-fit all mechanical components and mounts before final assembly

## 2. Wiring
2.1 Solder headers (if necessary) and prepare power wires for ESP32 and modules
2.2 Connect power from the Li-ion battery pack to the buck converter and 12V peripherals
2.3 Connect 5V and 3.3V power rails from the buck converter and ESP32 to respective modules
2.4 Wire data lines between the ESP32 and sensors/actuators (motors, servo, sensors, switch)
2.5 Connect motors and the linear actuator to the motor driver and battery directly
2.6 Connect the water pump and solenoid valve to the pump relay and battery
2.7 Perform continuity checks on all power and ground connections to prevent shorts

See [`hardware/electrical_connections.json`](../hardware/electrical_connections.json) for the
full wire-by-wire connection list (source, target, voltage, current).

## 3. Bring-up
3.1 Upload basic test firmware to the ESP32 for GPIO verification
3.2 Test the motor driver and individual drive motors (L/R) with simple code to confirm movement and direction
3.3 Verify servo operation, linear actuator movement, and limit switch functionality
3.4 Test the pump relay, water pump, and solenoid valve activation
3.5 Read data from the rotary encoder, soil moisture sensor, GPS module, and ultrasonic sensor
3.6 Calibrate sensor readings and adjust parameters as needed (e.g., GPS baud rate, ultrasonic range)

## 4. Assembly
4.1 Assemble the chassis structure using the base plate, side panels, front, and rear panels
4.2 Mount drive motors into their mounts, attach motor shaft couplers and wheels to axles, then mount the assembly to the chassis
4.3 Install the seed hopper, seed metering plate, seed sowing servo, and seed funnel
4.4 Mount the plough linear actuator, tilling blade, and limit switch to the chassis
4.5 Install the spraying liquid tank, water pump, solenoid valve, nozzle, and connect all water hoses
4.6 Mount all electronic components (ESP32, motor driver, relay, sensors, GPS) to their mounts and chassis
4.7 Route and secure all electrical wiring, applying strain relief and cable ties where appropriate
4.8 Mount the main battery pack to the chassis, ensuring secure placement

See [`hardware/mechanical_connections.json`](../hardware/mechanical_connections.json) for the
full part-to-part mechanical attachment list.

> **Note:** Detailed sub-step instructions (torque values, exact hose lengths, wiring diagrams
> per connector) are not yet written up — this guide currently covers the task sequence. PRs
> filling in the detail are welcome.
