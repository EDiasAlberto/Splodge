#include <WiFi.h>



const int barkPin = 26;
const int walkPin = 25;

const char* ssid = "";
const char* password = "";

WiFiServer server(420); //Creates the webserver object on port 420

String header;

unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 5000;


const char* PARAM_INPUT_1 = "duration";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Smart Timer</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Duration: <input type="text" name="duration">
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";


void buttonPress(int pin){
  for(int i=0;i<2;i++){
    Serial.printf("%i is High\n", pin);
    digitalWrite(pin, HIGH);
    delay(100);
    Serial.printf("%i is Low\n", pin);
    digitalWrite(pin, LOW);
    if(i<1){
      delay(10000);
    }
    
  }
  

  Serial.println("Sleeping...");
  
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
  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {                    
		
          if (currentLine.length() == 0) {

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            

            if (header.indexOf("GET /25") >= 0) {
              Serial.println("GPIO 25 on");
              buttonPress(barkPin);
            } else if (header.indexOf("GET /26") >= 0) {
              buttonPress(walkPin);
            } 
            

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");

            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            

            client.println("<body><h1>ESP32 Web Server</h1>");
    
            client.println("<p><a href=\"/25\"><button class=\"button\">Bark</button></a></p>");
     
            client.println("<p><a href=\"/26\"><button class=\"button\">Walk</button></a></p>");
            
            client.println("</body></html>");

            client.println();

            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }

    header = "";

    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  
}
