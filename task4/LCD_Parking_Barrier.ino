#include <Servo.h>
#include <LiquidCrystal.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(11, 10, A0, A1, A2, A4);

// create servo object
Servo myservo;

// variables
int pos = 0;
const int trigPin = 7;
const int echoPin = 6;
int potPin = A3;
int potVal = 0;

float duration, distance;
float threshold_cm;

void setup() {
  myservo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);

  // LCD setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Barrier System");
  lcd.setCursor(0, 1);
  lcd.print("Initialising...");
  delay(1500);
}

void loop() {
  myservo.write(pos);

  // Trigger ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.0343) / 2;

  // Read potentiometer
  potVal = analogRead(potPin);
  threshold_cm = map(potVal, 0, 1023, 5, 100);

  // Print threshold and barrier state
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thresh:");
  lcd.print(threshold_cm);
  lcd.print("cm");

  lcd.setCursor(0, 1);
  lcd.print("Barrier: ");
  lcd.print((distance < threshold_cm) ? "OPEN " : "CLOSED");

  // If distance is smaller than threshold → open barrier
  if (distance < threshold_cm) {
    for (pos = 0; pos <= 90; pos += 1) {
      myservo.write(pos);
      delay(15);
    }

    delay(5000); // wait for car to pass

    for (pos = 90; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }

    delay(1000);
  }
}
