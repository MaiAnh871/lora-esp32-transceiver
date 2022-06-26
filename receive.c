#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <string.h>
#include <DHT.h>
const char* ssid = "VIETTEL";
const char* password = "14112001" 
//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
String UrlThingspeak = "https://api.thingspeak.com/update?api_key=AN40V4YBIDEKLYTN"
String httpGETRequest(const char* Url);
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("conecting");
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
 
  while (!LoRa.begin(433E6)) {
    Serial.println(",");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
    }
  
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    char para[50];
    sprintf(para,"&field1=%6.2f&field2=%6.2f",t,h);
    String Url = UrlThingspeak + String(para);
    httpGETRequest(Url.c_str());
  }
}
String httpGETRequest(const char* Url)
{
  HTTPClient http;
  http.begin(Url);
  int responseCode = http.GET();
  String responseBody = "{}";
  if(responseCode > 0)
  {
    Serial.print("responseCode:");
    Serial.println(responseCode);
    responseBody = http.getString();
  }
  else
  {
    Serial.print("Error Code: ");
    Serial.println(responseCode);
  }
  http.end();
  return responseBody;
}