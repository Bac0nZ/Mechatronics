#include <Wire.h>
#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 10, A0, A1, A2, A4);

// Pin definitions
int red = 5;
int yellow = 4;
int green = 3;

int switchState = 0;
int lastSwitchState = 0;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

const int pingPin = 7;
const int echoPin = 6;

void setup() {
  Serial.begin(9600);

  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(2, INPUT);

  // LCD setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
}

void loop() {
  int reading = digitalRead(2);

  if (reading != lastSwitchState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != switchState) {
      switchState = reading;
    }
  }
  lastSwitchState = reading;

  if (switchState == HIGH) {
    pedestrianMode();
    return;
  }

  int distance = calculateDistance(pingPin, echoPin);

  if (distance <= 100) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cars: GREEN");
    lcd.setCursor(0, 1);
    lcd.print("Ped: WAIT");

    delay(1000);

    while (distance <= 100) {
      distance = calculateDistance(pingPin, echoPin);
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      digitalWrite(green, HIGH);

      if (digitalRead(2) == HIGH) {
        pedestrianMode();
        return;
      }
      delay(100);
    }
  }

  if (distance > 100) {
    digitalWrite(red, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cars: RED");
    lcd.setCursor(0, 1);
    lcd.print("Ped: WAIT");

    delay(1000);

    while (distance > 100) {
      distance = calculateDistance(pingPin, echoPin);
      digitalWrite(red, HIGH);
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);

      if (digitalRead(2) == HIGH) {
        pedestrianMode();
        return;
      }
      delay(100);
    }
  }
}

void pedestrianMode() {
  digitalWrite(red, LOW);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pedestrian Mode");
  delay(1000);

  digitalWrite(red, HIGH);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);

  for (int i = 10; i >= 1; i--) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cross Now!");
    lcd.setCursor(0, 1);
    lcd.print("Time Left: ");
    lcd.print(i);
    delay(1000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done");

  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

int calculateDistance(int pingPin, int echoPin) {
  long duration, cm;

  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);

  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = microsecondsToCentimeters(duration);
  return cm;
}
