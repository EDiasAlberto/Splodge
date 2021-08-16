#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebSocketServer.h>



const int barkPin = 26;
const int walkPin = 25;
const char* ssid = "";
const char* password = "";

WiFiServer server(420);
WebSocketServer webSocket; //Creates the webserver object on port 420

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 5000;




void buttonPress(int pin){
  Serial.printf("%i is High\n", pin);
  digitalWrite(pin, HIGH);
  delay(100);
  Serial.printf("%i is Low\n", pin);
  digitalWrite(pin, LOW);
}

void handleData(String message){
  Serial.println(message);
  if(message == "A"){
    buttonPress(walkPin);
  } else if(message=="B"){
    buttonPress(barkPin);
  } else {
    Serial.println("Invalid input");
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(barkPin, OUTPUT);
  pinMode(walkPin, OUTPUT);
  digitalWrite(barkPin, LOW);
  digitalWrite(walkPin, LOW);

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
         handleData(data);

      }
 
     
   }
 
   Serial.println("The client disconnected");
   delay(100);
  }
 
  delay(100);
  
}
