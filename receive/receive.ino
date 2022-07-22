#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <string.h>

const char* ssid = "SetStaff";
const char* password = "giangvien2015";
const char* API_KEY = "KT4EU7OP7BKE0BQX";
//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
String httpGETRequest(const char* Url);

void setup()
{
  //initialize Serial Monitor
  Serial.begin(115200);
  WiFi.begin(ssid, password);
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
 
  while (!LoRa.begin(866E6))
  {
    Serial.println(",");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {

    while (LoRa.available())
    {
      String LoRaData = LoRa.readString();
      if ((WiFi.status() == WL_CONNECTED))
      {
        HTTPClient http;
        String URL_GET = "https://api.thingspeak.com/update?api_key=" + String(API_KEY)
                     + "&field1=" + LoRaData;

        erial.print("GET URL:");
        Serial.println(URL_GET);

        http.begin(URL_GET); //Specify the URL
        int httpCode = http.GET();                        //Make the request

        if (httpCode > 0)
        { //Check for the returning code

        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        }
        else
        {
        Serial.println("Error on HTTP request");
        }

        http.end(); //Free the resources
      }
    }
  
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    delay(20000);//Min is 15second
  }
}
