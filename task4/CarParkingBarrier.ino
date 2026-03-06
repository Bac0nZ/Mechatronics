//inlcludes code from https://docs.arduino.cc/tutorials/generic/basic-servo-control/
//and https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380

#include <Servo.h>

Servo myservo;  
// create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    
// variable to store the servo position

const int trigPin = 7;
const int echoPin = 6;

float duration, distance;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  myservo.write(pos);
  //if ultrasound distance >20cm 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  
  if (distance < 20)
  {
    for (pos = 0; pos <= 90; pos += 1)
    { // goes from 0 degrees to 90 degrees
      // in steps of 1 degree
      Serial.println("moving");
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15); 
    }
    delay(5000);
    for (pos = 90; pos >= 0; pos -= 1) 
    { // goes from 90 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);// waits 15ms for the servo to reach the position
    }// waits 15ms for the servo to reach the position
    delay(1000);    
  }
}
 
