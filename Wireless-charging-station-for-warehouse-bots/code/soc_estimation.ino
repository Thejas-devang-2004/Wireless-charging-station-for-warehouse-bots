const float BATTERY_CAPACITY_Ah = 1.2;    // Example: 2200mAh = 2.2Ah 
const float NOMINAL_VOLTAGE = 12.5;         // Nominal voltage 

 
29 
const float FULLY_CHARGED_VOLTAGE = 12.86;   // Voltage at 100% SOC 
const float CUTOFF_VOLTAGE = 11.5;          // Voltage at 0% SOC 
 
// --- Voltage sensor setup --- 
const int voltageSensorPin = A2;           // Voltage sensor pin 
const float factor = 5.128;                // Voltage divider reduction factor 
const float vCC = 12.8;                    // Arduino reference voltage (usually 5V) 
 
float voltageSensorVal; 
float vIn; 
float vOut; 
// --- SOC variables --- 
float SOC = 100.0;         // Start SOC 
unsigned long lastTime = 0; // For timing 
// --- Variables for measured values --- 
float voltageM = 0.0; 
float currentM = 0.0; 
float voltage() { 
  voltageSensorVal = analogRead(voltageSensorPin); 
  vOut = (voltageSensorVal / 1024.0) * vCC; 
  vIn = vOut * factor; 
  return vIn; 
} 
// --- Current reading function --- 
float current() { 
  float sum = 0.0; 
  for (int i = 0; i < 100; i++) { 
    sum += (0.0264 * analogRead(A1) - 13.51) / 1000.0; 
  } 
  float averageCurrent = sum / 100.0 
  return averageCurrent; 

 
30 
} 
// --- SOC Update function --- 
void updateSOC(float currentMeasured, float voltageMeasured) { 
  unsigned long currentTime = millis(); 
  float deltaTimeHours = (currentTime - lastTime) / 3600000.0;  // ms to hours 
  lastTime = currentTime; 
  // Coulomb Counting 
  SOC -= (currentMeasured * deltaTimeHours) / BATTERY_CAPACITY_Ah * 100.0; 
  // Voltage Correction if resting 
  if (abs(currentMeasured) < 0.05) { // Near idle 
    float estimatedSOC = mapVoltageToSOC(voltageMeasured); 
    SOC = 0.9 * SOC + 0.1 * estimatedSOC; // Blend 
  } 
  // Clamp SOC 
  SOC = constrain(SOC, 0.0, 100.0); 
} 
// --- Map voltage to SOC --- 
float mapVoltageToSOC(float voltageMeasured) { 
  if (voltageMeasured >= FULLY_CHARGED_VOLTAGE) return 100.0; 
  if (voltageMeasured <= CUTOFF_VOLTAGE) return 0.0; 
  return (voltageMeasured - CUTOFF_VOLTAGE) / (FULLY_CHARGED_VOLTAGE - 
CUTOFF_VOLTAGE) * 100.0; 
} 
 
 
 
 
 
 
 
 

 
