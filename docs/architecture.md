# Architecture

```text
Sensors -> validation and filtering -> interactive monitor -> output/alert
                                      |
                                      +-> BLE telemetry and logs
```

The implementation separates acquisition, decision logic, output handling, and telemetry. Hardware-specific access is kept at the edge so the core behavior can be tested with simulated readings.
