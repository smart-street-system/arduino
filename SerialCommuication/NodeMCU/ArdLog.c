#include<SoftwareSerial.h>
SoftwareSerial mySerial (D1, D2); // TX and RX pins

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);
}

void loop() {
  String msg = mySerial.readStringUntil('\r');
  if (msg && msg.length()) {
    // LED DEFECTED
    if (msg[0] == 'D') {
      
    } else if (msg[0] == 'V') { // LED STATE CHANGES LOGS
      if (msg[8] == 'N') { // LED STATE ON
        
      } else if (msg[8] == 'F') { // LED STATE OFF
        
      }
    }
  }
}