/*
 *  Author: Rajkumar R
 *  Date: 19 Feb 2022
 *  lisence: Propiertory
 *  
 *  About: This program is for detection of fault in LED and turning ON the backup LED on failure of main LED
 *  Note: The LED glows only if it detects a object within 7cm and dark
 */

// On start of the Arduino run these
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Hello from Arduino");
  delay(1000);
}