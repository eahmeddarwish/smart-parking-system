/*
 * Smart Parking System
 * --------------------
 * A 15-slot car park (3 sections x 5 slots) on an Arduino Mega. Each slot has
 * an IR sensor and a Red/Green LED pair; entrance and exit IR sensors drive two
 * servo gates. A 20x4 I2C LCD shows how many slots are free in each section.
 *
 *   slot occupied -> RED  on the slot LED
 *   slot free     -> GREEN on the slot LED
 *   car at entrance/exit -> raise the gate for a few seconds, then close
 *
 * Rewritten from a flat, 200-line copy-paste version into arrays + a slot map,
 * so 15 slots take one loop instead of 15 repeated blocks.
 *
 * Author : Ahmed Darwish  <eahmeddarwish@gmail.com>
 * License: MIT
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// ------------------------------- CONFIG ---------------------------------- //
const int SLOTS = 15;
const int SECTIONS = 3;                 // 5 slots per section
const int SLOTS_PER_SECTION = 5;

// IR sensor pin for each slot (index 0..14)
const int slotSensor[SLOTS] = {5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 18, 19, 17, 20};
// Green / Red LED pin for each slot
const int slotGreen[SLOTS]  = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50};
const int slotRed[SLOTS]    = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51};

const int ENTRANCE_IR = 52, EXIT_IR = 53;
const int ENTRANCE_SERVO = 3, EXIT_SERVO = 4;
const int GATE_OPEN = 90, GATE_CLOSED = 0;
const unsigned long GATE_MS = 4000;

const bool IR_ACTIVE_LOW = true;        // most IR modules read LOW when blocked
// ------------------------------------------------------------------------- //

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo entranceGate, exitGate;

bool occupied(int pin) {
  int v = digitalRead(pin);
  return IR_ACTIVE_LOW ? (v == LOW) : (v == HIGH);
}

void setup() {
  for (int i = 0; i < SLOTS; i++) {
    pinMode(slotSensor[i], INPUT);
    pinMode(slotGreen[i], OUTPUT);
    pinMode(slotRed[i], OUTPUT);
  }
  pinMode(ENTRANCE_IR, INPUT);
  pinMode(EXIT_IR, INPUT);
  entranceGate.attach(ENTRANCE_SERVO); entranceGate.write(GATE_CLOSED);
  exitGate.attach(EXIT_SERVO);         exitGate.write(GATE_CLOSED);

  lcd.begin(); lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("   Parking System");
}

void loop() {
  handleGate(ENTRANCE_IR, entranceGate);
  handleGate(EXIT_IR, exitGate);

  int freePerSection[SECTIONS] = {0, 0, 0};
  for (int i = 0; i < SLOTS; i++) {
    bool taken = occupied(slotSensor[i]);
    digitalWrite(slotRed[i],  taken ? HIGH : LOW);
    digitalWrite(slotGreen[i], taken ? LOW  : HIGH);
    if (!taken) freePerSection[i / SLOTS_PER_SECTION]++;
  }

  showFree("First: ",  freePerSection[0], 1);
  showFree("Second: ", freePerSection[1], 2);
  showFree("Third: ",  freePerSection[2], 3);
  delay(300);
}

void handleGate(int irPin, Servo &gate) {
  if (occupied(irPin)) {
    gate.write(GATE_OPEN);
    delay(GATE_MS);
    gate.write(GATE_CLOSED);
  }
}

void showFree(const char* label, int freeCount, int row) {
  lcd.setCursor(0, row); lcd.print(label);
  lcd.setCursor(9, row); lcd.print(freeCount); lcd.print("/5 ");
}
