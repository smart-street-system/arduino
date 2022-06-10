/*
 *  Author: Rajkumar R
 *  Date: 19 Feb 2022
 *  lisence: Propiertory
 *  
 *  About: This program is for detection of fault in LED and turning ON the backup LED on failure of main LED
 *  Note: The LED glows only if it detects a object within 7cm and dark
 */

// List of PINS in Arduiuno
const int ledMainPin = 8;
const int ledBackupPin = 7;
const int ldrPin = 0;
const int pirPin = 2;
const int irPin = 3;
const int ledMainAnalogPin = 4;

// pins for Motion Sensor
const int echo = 13;
const int trig = 12;

// Motion sensor distance and duration of the object from sensor
int distance = 0;
int duration = 0;

// LDR and LED Analogog values accross the Sensors to check the value and predict the nature of failure
int ledMainAnalogValue = 0;
int ldrAnalogValue = 0;
int pirValue = 0;
int pirState = LOW;

// Developers Constants for working and configration of system
int LDRThreshlodValue = 100;
int dynamicDelayInMinutes = 1;
int speedup = 100; // 1000 for actual speed, < 1000 speeds up, > 1000 slows down

// state os failure of LED
int isLEDWorking = 1;


// On start of the Arduino run these
void setup() {
  Serial.begin(9600);
  pinMode(ledMainPin, OUTPUT);
  pinMode(ledBackupPin, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(irPin, INPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(ledMainPin, LOW);
  digitalWrite(ledBackupPin, LOW);
}

void motionSensorHandler() {
  // handler motion
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 28.5;
  Serial.println(distance);

  if (distance <= 6) {
    // let the LED be ON
  } else {
    digitalWrite(ledBackupPin, LOW);
    digitalWrite(ledMainPin, LOW);
  }
}

void turnLDRHandler(bool state) {
  if (state) {
    digitalWrite(ledMainPin, HIGH);
    ledMainAnalogValue = analogRead(ledMainAnalogPin);

    if (ledMainAnalogValue >= 300 && ledMainAnalogValue <= 600) {
      // LED Working
      isLEDWorking = 1;
      digitalWrite(ledBackupPin, LOW);
      motionSensorHandler();
      delay(1000);
    } else {
      // LED not working, send remote message
      // Turn ON LED Backup
      isLEDWorking = 0;
      digitalWrite(ledBackupPin, HIGH);
      digitalWrite(ledMainPin, LOW);
      motionSensorHandler();
      delay(1000);
    }
  } else {
    if (isLEDWorking) {
      digitalWrite(ledMainPin, LOW);
    } else {
      digitalWrite(ledBackupPin, LOW);
    }
  }
}


// runs always after start up
void loop() {
//  ldrAnalogValue = analogRead(ldrPin);
//
//  if (ldrAnalogValue <= 100) {
//    turnLDRHandler(true);
//  } else {
//    turnLDRHandler(false);
//    delay((ldrAnalogValue - LDRThreshlodValue) * dynamicDelayInMinutes * speedup);
//  }

//  PIR MOTION SENSOR
//  pirValue = digitalRead(pirPin);
//
//  if (pirValue == HIGH && digitalRead(irPin) == LOW) {
//    Serial.println("LED ON");
//    digitalWrite(ledMainPin, HIGH);
//    delay(5000);
//  } else {
//    Serial.println("LED OFF");
//    digitalWrite(ledMainPin, LOW);
//  }

// IR SENSOR
//  if (digitalRead(irPin) == HIGH) {
//    digitalWrite(ledMainPin, HIGH);
//  } else {
//    digitalWrite(ledMainPin,LOW);
//  }

  Serial.println("Hello from Arduino");
  delay(1000);
}