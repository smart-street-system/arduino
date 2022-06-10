#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include<SoftwareSerial.h>

const char *ssid = "RAJKUMAR_AIRTEL";
const char *password = "baba1206";

const char *host = "smart-street-lamp.herokuapp.com";
const int httpsPort = 443;

const char fingerprint[] PROGMEM = "34 6a 4f 35 49 d7 8b c1 ea 74 b9 9e c9 a8 c9 1f 5e 6c 27 61";

SoftwareSerial mySerial (D1, D2); // TX and RX pins

String lampName ="testLamp";
String lamp_lng ="12.1235113377";
String lamp_lat ="11.235113377";
String lamp_active ="true";
String _id = "";


void registerLamp() {
  // MAKE SERVICECALL
  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient

  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(25000); // 25 Seconds
  delay(1000);
  
  Serial.println("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  
  String getData, Link;
  
  //POST Data
  Link = "/api/lamps";
  String body ="lampName="+lampName+"&longitude="+lamp_lng+"&lattitude="+lamp_lat+"&active="+lamp_active;

  Serial.print("requesting URL: ");
  Serial.println(host);

  Serial.println(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded"+ "\r\n\r\n" +
               body + "\r\n"+
               "Connection: close\r\n\r\n");
          
  httpsClient.println(String("POST ") + Link + " HTTP/1.1");
  httpsClient.print("Host: "); httpsClient.println(host);
  httpsClient.println("Content-Type: application/x-www-form-urlencoded");
  httpsClient.print("Content-Length: "); httpsClient.println(body.length());
  httpsClient.println();    // extra `\r\n` to separate the http header and http body
  httpsClient.println(body);

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
    
    int ind = -1;
    for (int i = 0; i < line.length() - 2; i++) {
      if (line[i] == '_' && line[i+1] == 'i' && line[i+2] == 'd') {
        ind = i;
        break;
      }
    }
    Serial.println(ind);
    if (ind != -1) {
      for(int i = 0; i < 24; i++) {
        _id += (String)line[ind+i+6];
      }
      Serial.println(_id);
    }
  }
}

void lampDefected() {
  // LAMP DEFECTED
  Serial.println("[nodemcu] LAMP DEFECTED");
  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient

  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  httpsClient.setFingerprint(fingerprint);
  httpsClient.setTimeout(25000); // 25 Seconds
  delay(1000);
  
  Serial.println("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  
  String getData, Link;
  
  //POST Data
  Link = "/api/lamps/" + _id;
  String body ="lampName="+lampName+"&isActive=false";

  Serial.print("requesting URL: ");
  Serial.println(host);

  Serial.println(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded"+ "\r\n\r\n" +
               body + "\r\n"+
               "Connection: close\r\n\r\n");
          
  httpsClient.println(String("POST ") + Link + " HTTP/1.1");
  httpsClient.print("Host: "); httpsClient.println(host);
  httpsClient.println("Content-Type: application/x-www-form-urlencoded");
  httpsClient.print("Content-Length: "); httpsClient.println(body.length());
  httpsClient.println();    // extra `\r\n` to separate the http header and http body
  httpsClient.println(body);

  Serial.println("request sent");

  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  Serial.println("reply was:");
  Serial.println("==========");
  String line;
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
    Serial.println(line); //Print response
  }
}

void lampLog(int state) {
  // MAKE SERVICECALL
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  registerLamp();

  // listen on Arduino Freq
  mySerial.begin(9600);
}

void loop() {
  String msg = mySerial.readStringUntil('\r');
  if (msg && msg.length()) {
    // LED DEFECTED
    if (msg[0] == 'D') {
      lampDefected();
    } else if (msg[0] == 'V') { // LED STATE CHANGES LOGS
      if (msg[8] == 'N') { // LED STATE ON
        lampLog(1);
      } else if (msg[8] == 'F') { // LED STATE OFF
        lampLog(0);
      }
    }
  }
}