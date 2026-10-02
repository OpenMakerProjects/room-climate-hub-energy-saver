# Room Climate Hub Energy Saver

Build a smart home prototype that uses OLED display, door sensor to reduce unnecessary power use. Include setup instructions, a circuit diagram, tested firmware, and sample output.

## Project details

| Field | Value |
| --- | --- |
| Roadmap ID | 4 |
| Category | Smart Home |
| Platform | ESP8266 |
| Difficulty | Advanced |
| Estimated build time | 52 hours |
| Connectivity | BLE |
| Core components | OLED display, door sensor |
| Control mode | interactive monitor |

## Repository layout

- `firmware/room-climate-hub-energy-saver/room-climate-hub-energy-saver.ino`: runnable firmware or application
- `docs/wiring.md`: suggested low-voltage wiring plan
- `docs/architecture.md`: system data flow
- `docs/test-plan.md`: repeatable verification steps
- `sample-data/example.json`: example telemetry record
- `tools/validate.py`: dependency-free repository validation

## Quick start

1. Open `firmware/room-climate-hub-energy-saver/room-climate-hub-energy-saver.ino` in Arduino IDE or Arduino CLI.
2. Select the board matching **ESP8266**.
3. Compile and upload, then open the serial monitor at 115200 baud.

## Expected behavior

Energy Saver demonstration with repeatable test steps. The default implementation supports simulated or generic analog inputs so the control path can be exercised before hardware-specific drivers are added.

## Hardware adaptation

The included code is a safe reference implementation. Update pin assignments and sensor conversions from the exact component datasheets, then repeat the test plan before connecting actuators.

## License

MIT
