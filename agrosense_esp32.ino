// ===== PIN CONFIG =====
const int soilPin = 34;
const int thermPin = 35;

// ===== CALIBRATION (UPDATED FOR YOUR VALUES) =====
int dryValue = 350;   // sensor in air (you observed ~300+)
int wetValue = 80;    // sensor in wet soil (~80–100)

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  delay(1000);
}
// ===== AVERAGE FUNCTION =====
int readAverage(int pin) {
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(pin);
    delay(10);
  }
  return total / 10;
}

// ===== MOISTURE FUNCTION =====
int getMoisturePercent(int rawValue) {
  int percent = map(rawValue, dryValue, wetValue, 0, 100);
  return constrain(percent, 0, 100);
}

// ===== TEMPERATURE FUNCTION (FOR SENSOR MODULE) =====
float getTemperature() {
  int raw = readAverage(thermPin);

  float voltage = raw * (3.3 / 4095.0);

  if (voltage <= 0.1) return 0;

  float resistance = (3.3 - voltage) * 10000 / voltage;

  float tempK = 1 / (log(resistance / 10000) / 3950 + 1 / 298.15);
  float tempC = tempK - 273.15;

  // 🔥 Calibration offset fix
  float correctedTemp = tempC + 14;

  return correctedTemp;
}

// ===== LOOP =====
// ===== LOOP (Dashboard Compatible) =====
void loop() {
  int soilRaw = readAverage(soilPin);
  int moisturePercent = getMoisturePercent(soilRaw);
  float temperature = getTemperature();

  // Print as JSON for the AgroSense Dashboard
  Serial.print("{\"moisture\": ");
  Serial.print(moisturePercent);
  Serial.print(", \"temp\": ");
  Serial.print(temperature);
  Serial.println("}");

  delay(2000); 
}