#include <Arduino.h>
#include <WiFi.h>
#include <HttpClient.h>
#include <WebServer.h>

WebServer server;
char* gereateName = "Portemonnaie";
const char* ssid = "Wlantag";
const char* password = nullptr; 
unsigned int  localUdpPort = 8001;
//IPAddress IP_Remote(192, 168, 43, 132);
const int led = 12;
const int tasterIn = 14;
bool ledleuchtet = false;

void a() { 
ledleuchtet=true;
} 

void pingback() { 
server.send(200, "ping");
}

void setup() {
  Serial.begin(9600);
  while(!Serial);
  pinMode(led,OUTPUT);
  pinMode(tasterIn,INPUT_PULLUP);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected");

  HTTPClient client;
  client.begin("http://192.168.43.132:8000/tags/" + WiFi.macAddress() + "/" + gereateName);
  client.POST("");
  client.end();

Serial.print(WiFi.localIP());

server.begin(8000);
server.on("/alarm", a);
server.on("/status", pingback);
}


void loop() {

server.handleClient();

if (ledleuchtet) {    
    digitalWrite(led,HIGH);
    delay(200);
    digitalWrite(led,LOW);
    delay(200);
  }  

if (digitalRead(tasterIn)==LOW) {    
    ledleuchtet = false;
  }
}