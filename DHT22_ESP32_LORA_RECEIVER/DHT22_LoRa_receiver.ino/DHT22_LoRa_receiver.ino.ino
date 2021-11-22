#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);
  Serial.println("LoRa Receiver");
  LoRa.setPins(18,23,26);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing Complete!");
}

void loop() {
  // put your main code here, to run repeatedly:
  String LoRaData="";
  int packetSize = LoRa.parsePacket(); // Check if any data has been received by LoRa module

  if (packetSize) { // if >0 then yes received a packet
    // received a packet
    Serial.print("Received packet '");

    // Now read the packet
    while (LoRa.available()) {  
      LoRaData = LoRa.readString();
    }
    Serial.print(LoRaData); //Print the received data

    
    Serial.print("with RSSI ");
    Serial.println(LoRa.packetRssi());
    Serial.print("\n");
    
  }
 // Serial.print("Marche");
}

/////OOOH Faire une string complete dans le sender PUIS envoyer via LoRa Print !! OU faire plein de string
