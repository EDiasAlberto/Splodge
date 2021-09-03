#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketServer.h>
#include <ESP32Servo.h>

Servo servo1, servo2;

int pos1 = 90, pos2 = 90;

const int servoPin1 = 26;
const int servoPin2 = 25;
const char* ssid = "TALKTALK48237E";
const char* password = "8Y4ETH64";

WiFiServer server(420);
WebSocketServer webSocket; //Creates the webserver object on port 420

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 5000;


void handleData(String message){
  if(message=="LT"){
    pos1 = min(pos1+10, 180);
    servo1.write(pos1);
  } else if(message=="RT"){
    pos2 = min(pos2+10, 180);
    servo2.write(pos2);
  } else if(message=="A"){
    pos1 = max(pos1-10, 0);
    servo1.write(pos1);
  } else if(message=="B"){
    pos2 = max(pos2-10, 0);
    servo2.write(pos2);
  } else {
    Serial.println("Invalid button");
  }
}


void setup() {
  Serial.begin(115200);

  servo1.attach(servoPin1, 500, 4000);   
  servo1.write(90);

  servo2.attach(servoPin2, 500, 4000);   
  servo2.write(90);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

}


void loop() {
  WiFiClient client = server.available();
 
  if (client.connected() && webSocket.handshake(client)) {
 
    String data;      
 
    while (client.connected()) {
 
      data = webSocket.getData();
 
      if (data.length() > 0) {
         webSocket.sendData("Hello.");
         handleData(data);
         webSocket.sendData((String)pos1);
         webSocket.sendData((String)pos2);

      }
 
     
   }
 
   Serial.println("The client disconnected");
  }
  delay(10);
 
}
