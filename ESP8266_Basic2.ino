#include <ESP8266WiFi.h>

const char *ssid = "[SSID]";
const char *pass = "[PASS]";

WiFiClient client;

IPAddress server_ip = {192,168,1,39};
const int PORT = 8000;

void setup() {
  Serial.begin(112500);

  WiFi.begin(ssid,pass);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  } Serial.println();

  Serial.print("Connected : ");
  Serial.println(WiFi.localIP());

  Serial.print("Open TCP Client");
  while(!client.connect(server_ip,PORT)){
    Serial.print(".");
    delay(500);
  } 
  Serial.print(" -> ");
  Serial.print(server_ip[0]); Serial.print(".");
  Serial.print(server_ip[1]); Serial.print(".");
  Serial.print(server_ip[2]); Serial.print(".");
  Serial.println(server_ip[3]);
}

void loop() {
  while(client.available()){
    byte data = client.read();
    Serial.write(data); 
    if(client.available() == 0) Serial.println();
  }
  
  while(Serial.available()){
    byte data = Serial.read();
    client.write(data);
    if(Serial.available() == 0) client.println();
  }
}
