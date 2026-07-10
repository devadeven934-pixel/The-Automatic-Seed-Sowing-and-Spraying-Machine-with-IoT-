🚜 Agri-Bot IoT Project Overview :

The Agri-Bot IoT is an innovative, battery-powered, four-wheel-drive agricultural robot engineered to automate three of the most labor-intensive pre-harvest operations on small and mid-sized farms: ploughing, seed sowing, and spraying. Operating under a single ESP32 microcontroller, this mechatronic system combines structural hardware with automation to execute field tasks with minimal human supervision.
The physical platform features an aluminum-paneled chassis propelled by two 12V DC geared motors via an L298N dual H-bridge driver, using differential steering to navigate field rows seamlessly. For primary soil preparation, a 12V linear actuator lowers a tilling blade into the ground, halting automatically when an integrated micro limit switch senses that the target depth of 50mm has been reached. Following the tilling assembly, a digital servo-metered hopper coordinates precise seed delivery. By tracking exact distance data from a wheel-mounted rotary encoder, the firmware releases a uniform seed charge at fixed intervals, maintaining a spacing accuracy of $\pm 2\text{–}3 \text{ cm}$ regardless of variations in driving speed. Simultaneously, the fluid dynamics module—comprising a 12V diaphragm pump and a normally-closed solenoid valve—manages localized liquid irrigation or chemical pesticide application through an adjustable nozzle spanning a 30 to 40 cm coverage width.  
What elevates this system from a standard mechanical implement is its continuous edge sensing and wireless connectivity framework. Onboard sensing arrays—including a capacitive soil moisture sensor, a NEO-6M GPS module, and an HC-SR04 ultrasonic sensor—constantly poll field variables to feed a central sense-decide-actuate loop. Safety is maintained through a hardcoded interlock loop: if the ultrasonic sensor detects an obstacle within 25 cm, the robot immediately forces a hardware stop across all operational configurations.  
Leveraging the ESP32’s Wi-Fi capabilities, the robot transmits structured JSON telemetry payloads over MQTT to a secure cloud platform every two seconds. This communication layer passes live coordinate locations, soil properties, and actuator status strings directly to a remote interface, allowing an operator to monitor field data logs or send immediate command overrides from a web dashboard. Ultimately, Agri-Bot IoT bridges the gap between hardware automation and data-driven precision agriculture, offering smallholders a cost-effective alternative to multi-pass machinery and manual labor.  
Agri-Bot IoT operates on a continuous sense → decide → actuate → report loop, executed by an ESP32 microcontroller roughly every control cycle:
Sense — Onboard sensors read the robot's physical state: a wheel-mounted rotary encoder tracks distance travelled, an HC-SR04 ultrasonic sensor measures distance to the nearest obstacle, a capacitive sensor reads soil moisture, a limit switch reports whether the plough has reached tilling depth, and a NEO-6M GPS module reports position.
Decide — Firmware evaluates the currently active mode (idle, manual, plough, sow, spray, or auto) against that sensor data. The logic is deliberately simple and safety-first: regardless of mode, if the ultrasonic sensor reports an obstacle inside the safety threshold, driving is halted immediately — the obstacle check overrides whatever task is running.
Actuate — Based on the decision, the ESP32 drives its outputs: PWM and direction signals to the L298N motor driver for movement, a pulse to the seed-metering servo when the encoder shows the configured spacing has been covered, extend/retract signals to the plough's linear actuator, and on/off signals to the spray pump relay and solenoid valve.
Report — Every two seconds, the robot assembles its current state — mode, GPS fix and coordinates, soil moisture, obstacle distance, distance travelled, plough limit-switch state, and spray activity — into a JSON packet and publishes it over MQTT, so an operator can monitor the robot without being physically present.

Applications:

Small and mid-sized farm plots
The core use case — a single robot handles tilling, seeding, and spraying across a row-cropped plot in one coordinated pass, replacing three separate manual or mechanical operations. Best suited to farms too small to justify a tractor-mounted seeder/sprayer but too large for fully manual work to stay efficient.
Kitchen gardens and community farming projects
The chassis size and low power draw make it practical for allotments, community gardens, and shared urban farming plots where consistent seed spacing and controlled watering matter but heavy machinery doesn't fit.
Nurseries and seed-testing plots
Repeatable, encoder-controlled seed spacing is directly useful for germination trials and nursery seed beds, where uniformity between rows affects the validity of comparisons.
Agricultural research
Because every action (plough depth reached, seed drop, spray activation, GPS position) is logged via MQTT telemetry, the robot naturally produces a timestamped, geolocated record of field operations — useful for research plots studying spacing, irrigation timing, or input application effects.
Greenhouse and polytunnel automation
The obstacle-safe, low-speed driving profile suits confined, structured environments like greenhouse aisles, where predictable rows and short distances reduce the need for advanced navigation.
Future Applications
Contract farming / shared-equipment services
A single robot (or small fleet) could be rented or shared across several smallholders in a region — similar to how contract tractor services work today — with the MQTT dashboard letting an operator manage multiple bookings and fields remotely.
Precision, variable-rate application
Combined with the existing soil moisture and GPS data, future firmware could vary seed spacing, plough depth, or spray volume by zone within a single field, rather than applying one fixed rate across the whole area.
Fleet operation
Because each robot publishes to its own MQTT topic (agribot/<id>/...), running multiple units in parallel across different plots — all visible on one dashboard — is a natural extension with no architectural changes needed.
Remote / off-grid smallholdings
Paired with solar charging (listed in the Future Scope), the robot could operate with minimal supervision in areas with limited labour access or unreliable grid power, checking in over Wi-Fi/cellular gateway when in range.
Military or humanitarian field agriculture
Rapidly deployable, low-cost automated planting/spraying units have been explored in adjacent open-source robotics projects (e.g. AgriCruiser) for post-disaster or resource-constrained agricultural recovery — a plausible long-term extension given the platform's low cost and modularity.
