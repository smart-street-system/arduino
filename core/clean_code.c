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

  digitalWrite(ledMainPin, LOW);
  digitalWrite(ledBackupPin, LOW);
}

void turnLEDHandler(bool state) {
  if (state) {
    digitalWrite(ledMainPin, HIGH);
    ledMainAnalogValue = analogRead(ledMainAnalogPin);

    if (ledMainAnalogValue >= 300 && ledMainAnalogValue <= 600) {
      // LED Working
      isLEDWorking = 1;
      digitalWrite(ledBackupPin, LOW);
    } else {
      // LED not working, send remote message
      // Turn ON LED Backup
      isLEDWorking = 0;
      Serial.println("V: LED DEFECTED");
      digitalWrite(ledBackupPin, HIGH);
      digitalWrite(ledMainPin, LOW);
    }
    Serial.println("V: LED ON");
  } else {
     digitalWrite(ledMainPin, LOW);
     digitalWrite(ledBackupPin, LOW);
     Serial.println("V: LED OFF");
  }
}

int is_on = 0;

void loop() {
  pirValue = digitalRead(pirPin);
  int flag = 1;
  if (pirValue == HIGH && digitalRead(irPin) == LOW) {
    // LED ON
    if (!is_on) {
      turnLEDHandler(true);
      is_on = 1;
    }
    flag = 1;
  } else {
    int c = 15;
    flag = 0;
    while(c > 0) {
      delay(300);
      if (digitalRead(pirPin) && digitalRead(irPin) == LOW) {
        flag = 1;
        turnLEDHandler(true);
        break;
      }
      c--;
    }
    if (flag == 0) {
      // LED OFF
      if (is_on) {
        turnLEDHandler(false);
        is_on = 0;
      }
    }
  }
}