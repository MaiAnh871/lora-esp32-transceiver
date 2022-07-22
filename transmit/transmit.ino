#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>
#define DHT_SENSOR_PIN  21 
#define DHT_SENSOR_TYPE DHT22
#define ss 5
#define rst 14
#define dio0 2
DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
int counter = 0;
float tg = 0;
void setup()
{
  dht_sensor.begin(); 
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6))
  {
    Serial.println(".");
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
  float humi  = dht_sensor.readHumidity();
  float tempC = dht_sensor.readTemperature();
  Serial.print("Sending packet: ");
  Serial.println(counter);
  if (isnan(tempC) || isnan(humi))
  {
    Serial.println("Failed to read from DHT sensor!");
  } 
  else
  {
    Serial.print("Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  ");

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C  ~  ");
  }
  
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  
  //LoRa.print(counter);
  tg= tempC;
  LoRa.print(tg);
  LoRa.endPacket();

  counter++;

  delay(30000);
}
