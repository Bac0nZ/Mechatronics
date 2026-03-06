// Pin Definitions
const int ldrPin = A0;   // LDR connected to analog pin A0
const int ledPin = 9;   // Internal LED or external LED on pin 13

// Threshold: Adjust this based on your room's brightness
// Lower value = needs to be darker to turn on
const int threshold = 400; 

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600); // Start serial monitor to see values
}

void loop() {
  int lightLevel = analogRead(ldrPin); // Read the LDR value (0-1023)

  Serial.print("Current Light Level: ");
  Serial.println(lightLevel);

  if (lightLevel < threshold) {
    digitalWrite(ledPin, HIGH); // It's dark! Turn LED on
  } else {
    digitalWrite(ledPin, LOW);  // It's bright! Turn LED off
  }

  delay(100); // Small delay for stability
}
