#include <Arduino.h>
#include <WiFi.h>

static char* ssid = "mintec2019";
static char* password = nullptr; 

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.print("Connecting");

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("Connected! %d\n", WiFi.isConnected());
  Serial.println("IP: "+ WiFi.localIP().toString());
  Serial.println("MAC-address: " + WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:
}