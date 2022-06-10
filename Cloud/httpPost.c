#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "RAJKUMAR_AIRTEL";
const char *password = "baba1206";

const char *host = "rajkumar-blog.herokuapp.com";
const int httpsPort = 443;

const char fingerprint[] PROGMEM = "d6 27 27 66 fb 8a 91 e6 31 3a b8 af b1 79 4b 8f c7 a6 5a dc";

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
  
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
}

void loop() {
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
  Link = "/blog/visit/";
  const char *body ="{\"name\":\"rajkumar\"}";
//  const char *body ="name:raj";

  Serial.print("requesting URL: ");
  Serial.println(host);
//  /*
//   POST /post HTTP/1.1
//   Host: postman-echo.com
//   Content-Type: application/json
//   Content-Length: 13
//  
//   say=Hi&to=Mom
//    
//   */

  Serial.println(String("POST ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Content-Type: application/json"+ "\r\n\r\n" +
               body + "\r\n"+
               "Connection: close\r\n\r\n");
          
httpsClient.println(String("POST ") + Link + " HTTP/1.1");
httpsClient.print("Host: "); httpsClient.println(host);
httpsClient.println("Content-Type: application/json");
httpsClient.print("Content-Length: "); httpsClient.println(strlen(body));
httpsClient.println();    // extra `\r\n` to separate the http header and http body
httpsClient.println(body);

     
//  httpsClient.print(String("POST ") + Link + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" +
//               "Content-Type: application/x-www-form-urlencoded"+ "\r\n" +
//               "Content-Length: 8" + "\r\n\r\n" +
//               body + "\r\n\r\n");

  Serial.println("request sent");
                  
//  while (httpsClient.connected()) {
//    String line = httpsClient.readStringUntil('\n');
//    if (line == "\r") {
//      Serial.println("headers received");
//      break;
//    }
//  }
//
//  Serial.println("reply was:");
//  Serial.println("==========");
//  String line;
//  while(httpsClient.available()){        
//    line = httpsClient.readStringUntil('\n');  //Read Line by Line
//    Serial.println(line); //Print response
//  }
//  Serial.println("==========");
//  Serial.println("closing connection");
    
  delay(5000);  //POST Data at every 2 seconds
}