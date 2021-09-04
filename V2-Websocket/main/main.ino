#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketServer.h>
#include <ESP32Servo.h>

Servo LXServo, LYServo;

int xpos = 90, ypos = 90;

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
  if(message[0] == 'L'){
    int posChange = message.substring(2).toInt();
    switch(message[1]) {
      case 'X':
        if(xpos + posChange > 180){
          xpos = 180;
        } else if(xpos + posChange < 0){
          xpos = 0;
        } else {
          xpos += posChange;
        }
        LXServo.write(xpos);
        break;
      case 'Y':
        if(ypos + posChange > 180){
          ypos = 180;
        } else if(ypos + posChange < 0){
          ypos = 0;
        } else {
          ypos += posChange;
        }
        LYServo.write(ypos);
        break;
      default:
        Serial.println("INVALID INPUT");
    }

  }
}


void setup() {
  Serial.begin(115200);

  LXServo.attach(servoPin1, 500, 4000);   
  LXServo.write(90);

  LYServo.attach(servoPin2, 500, 4000);   
  LYServo.write(90);

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
         webSocket.sendData((String)xpos);
         webSocket.sendData((String)ypos);

      }
 
     
   }
 
   Serial.println("The client disconnected");
  }
  delay(10);
 
}
