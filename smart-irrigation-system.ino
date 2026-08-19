#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// PIN DEFINITIONS
#define SOIL_PIN A0
#define RELAY_PIN 7

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// SOIL CALIBRATION
const int DRY_VALUE = 990;
const int WET_VALUE = 330;

// HYSTERESIS THRESHOLDS
const int PUMP_ON_THRESHOLD = 30;
const int PUMP_OFF_THRESHOLD = 50;

// Relay is ACTIVE LOW
const int RELAY_ON = LOW;
const int RELAY_OFF = HIGH;

// Stores current pump state
bool pumpState = false;

void setup() {

  Serial.begin(9600);

  pinMode(RELAY_PIN, OUTPUT);

  // Start with pump OFF
  digitalWrite(RELAY_PIN, RELAY_OFF);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");

  lcd.setCursor(0, 1);
  lcd.print("System Starting");

  delay(2000);

  lcd.clear();
}


void loop() {

  // READ SOIL SENSOR 
  int soilValue = analogRead(SOIL_PIN);

  // CONVERT ADC TO MOISTURE %
  int moisturePercent = map(
    soilValue,
    DRY_VALUE,
    WET_VALUE,
    0,
    100
  );

  // Keep percentage between 0 and 100
  moisturePercent = constrain(moisturePercent, 0, 100);

  // HYSTERESIS CONTROL
  
  // If soil becomes very dry
  if (moisturePercent <= PUMP_ON_THRESHOLD) {

    pumpState = true;
  }

  // If soil becomes sufficiently wet
  else if (moisturePercent >= PUMP_OFF_THRESHOLD) {

    pumpState = false;
  }

  // Between 30% and 50%:
  // Keep previous pump state
 
  // CONTROL RELAY
  if (pumpState) {

    digitalWrite(RELAY_PIN, RELAY_ON);

  } else {

    digitalWrite(RELAY_PIN, RELAY_OFF);
  }

  // SERIAL MONITOR

  Serial.print("ADC: ");
  Serial.print(soilValue);

  Serial.print(" | Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("%");

  Serial.print(" | ");

  if (pumpState) {

    Serial.println("DRY -> PUMP ON");

  } else {

    Serial.println("WET -> PUMP OFF");
  }

  // LCD DISPLAY
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moisturePercent);
  lcd.print("%   ");

  lcd.setCursor(0, 1);

  if (pumpState) {

    lcd.print("PUMP: ON ");

  } else {

    lcd.print("PUMP: OFF");
  }

  lcd.print("       ");


  delay(1000);
}