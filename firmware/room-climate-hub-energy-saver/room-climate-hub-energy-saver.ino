#include <Arduino.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Room Climate Hub Energy Saver
// Roadmap project 4; mode: interactive_monitor
constexpr uint8_t SENSOR_PINS[] = {A0, A1, A2};
constexpr size_t SENSOR_COUNT = sizeof(SENSOR_PINS) / sizeof(SENSOR_PINS[0]);
constexpr uint8_t OUTPUT_PIN = LED_BUILTIN;
constexpr unsigned long SAMPLE_INTERVAL_MS = 1750UL;
constexpr float TRIGGER_THRESHOLD = 0.49f;
constexpr uint8_t REQUIRED_CONFIRMATIONS = 2;

enum class SystemState : uint8_t { Starting, Normal, Active, Fault };

struct Snapshot {
  float values[SENSOR_COUNT];
  float score;
  bool valid;
};

SystemState state = SystemState::Starting;
unsigned long lastSampleAt = 0;
uint8_t confirmations = 0;
bool outputActive = false;

float normalizeReading(int raw) {
  return constrain(raw / 1023.0f, 0.0f, 1.0f);
}

Snapshot acquireSnapshot() {
  Snapshot snapshot{};
  snapshot.valid = true;
  float sum = 0.0f;
  for (size_t index = 0; index < SENSOR_COUNT; ++index) {
    const int raw = analogRead(SENSOR_PINS[index]);
    if (raw < 0) snapshot.valid = false;
    snapshot.values[index] = normalizeReading(raw);
    sum += snapshot.values[index];
  }
  snapshot.score = sum / SENSOR_COUNT;
  return snapshot;
}

bool decide(const Snapshot &snapshot) {
  if (!snapshot.valid) return false;
  const bool condition = snapshot.score >= TRIGGER_THRESHOLD;
  if (!condition) {
    confirmations = 0;
  } else if (confirmations < REQUIRED_CONFIRMATIONS) {
    confirmations += 1;
  }
  return confirmations >= REQUIRED_CONFIRMATIONS;
}

void applyOutput(bool requested, bool valid) {
  if (!valid) {
    outputActive = false;
    state = SystemState::Fault;
  } else {
    outputActive = requested;
    state = requested ? SystemState::Active : SystemState::Normal;
  }
  digitalWrite(OUTPUT_PIN, outputActive ? HIGH : LOW);
}

const char *stateName() {
  switch (state) {
    case SystemState::Starting: return "starting";
    case SystemState::Normal: return "normal";
    case SystemState::Active: return "active";
    default: return "fault";
  }
}

void publishTelemetry(const Snapshot &snapshot) {
  Serial.print(R"json({"project_id":4,"mode":"interactive_monitor","state":")json");
  Serial.print(stateName());
  Serial.print(R"json(","score":)json");
  Serial.print(snapshot.score, 3);
  Serial.print(R"json(,"output":)json");
  Serial.print(outputActive ? "true" : "false");
  Serial.print(R"json(,"values":[)json");
  for (size_t index = 0; index < SENSOR_COUNT; ++index) {
    if (index) Serial.print(',');
    Serial.print(snapshot.values[index], 3);
  }
  Serial.println("]}");
}

void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW);
  Serial.begin(115200);
  state = SystemState::Normal;
}

void loop() {
  const unsigned long now = millis();
  if (now - lastSampleAt < SAMPLE_INTERVAL_MS) return;
  lastSampleAt = now;
  const Snapshot snapshot = acquireSnapshot();
  applyOutput(decide(snapshot), snapshot.valid);
  publishTelemetry(snapshot);
}
