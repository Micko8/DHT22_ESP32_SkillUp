/*#include <Arduino.h>
#include <Adafruit_Sensor.h>*/
#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"

#define BAUDRATE 115200

#define DHTPIN 4 // 
#define DHTTYPE DHT22 //

#define CS 18
#define RST 23
#define IRQ 26

DHT dht(DHTPIN, DHTTYPE);

void Serial_init(int baudRate){
  Serial.begin(baudRate);
  delay(5000);// wait for the serial stream to be open before to print
  Serial.println("Lora Sender starting...");
}

void LoRa_init(int pinCS, int pinRST, int pinIRQ) {
  LoRa.setPins(pinCS, pinRST, pinIRQ); //LoRasetPins(18,14,26) = LoRasetPins(CS,RST,IRQ) in tuto

  if (!LoRa.begin(868E6)) {
    Serial.println("Could not find a valid LoRa transceiver, check pins used wiring");
    while (1);
  }
  
  LoRa.setSyncWord(0xF3);
  LoRa.setTxPower(20);
}

void DHT_init(){
  dht.begin(); //initialise DHT22
}

void setup() {
 // put your setup code here, to run once:
  Serial_init(BAUDRATE);

  DHT_init();

  LoRa_init(CS, RST, IRQ);
}

int counter = 0;

void loop() {

  // put your main code here, to run repeatedly
 //void DHT_read 
  float h = dht.readHumidity();
  //Celcius
  float t = dht.readTemperature();

  //check if any reads failed and exit early (to try again)
  if (isnan(h) || isnan(t))
  {
    LoRa.print("Failed to read from DHT sensor!");
    return;
  }
  
//void LoRa_print(float measure, String dataType)
  LoRa.beginPacket(); //Start LoRa transceiver
  Serial.println("Start of transmision");
  LoRa.print("Data:");
  LoRa.print(counter);
  LoRa.print("> ");
  LoRa.print("Temperature: ");
  Serial.println(t);
  LoRa.print(t);
  LoRa.print(" ");
  LoRa.print("Humidity: ");
  LoRa.print(h);
  LoRa.print("\n");
  LoRa.endPacket(); //Confirm end of LoRa data packet so receiver knows that's then end
  counter++;
  delay(5000);
  Serial.println("End of transmision");
  ////////////////////////////////////////////////////
  LoRa.sleep(); //Put LoRa receiver to sleep to save power
  Serial.println("Lora Sleep\n");
}
