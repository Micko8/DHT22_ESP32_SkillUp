/*
This example shows how to connect to Cayenne using an ESP32 and send/receive sample data.
The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.
Steps:
1. If you have not already installed the ESP32 Board Package install it using the instructions here: https://github.com/espressif/arduino-esp32/blob/master/README.md#installation-instructions.
2. Select your ESP32 board from the Tools menu.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/
#include <CayenneMQTTESP32.h>
#include "DHT.h"

#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define DHTPIN 4 // 
#define DHTTYPE DHT22 //

DHT dht(DHTPIN, DHTTYPE);

// WiFi network info.
char ssid[] = "Wellness WIFI";
char wifiPassword[] = "BuddhaBowl";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "8b437d10-4557-11ec-8da3-474359af83d7";
char password[] = "1cbe89e966150c10743f9a55ec4db20419db4e7c";
char clientID[] = "e9835970-4955-11ec-9f5b-45181495093e";

void setup() {
  Serial.begin(115200);
  dht.begin();             // To start communication with DHT sensor
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

}

void loop() {
  delay(2000);      // Give some delay between each Sensor reading
  Cayenne.loop();   
  float h = dht.readHumidity();     // To get Humidity reading
  float t = dht.readTemperature();  // To get temperature reading
  if (isnan(h) || isnan(t)) {       // Condition to check whether the sensor reading was successful or not
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");       // To print the data in serial monitor
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  Cayenne.virtualWrite(0, h);     // To write the Sensor reading to Cayenne Dashboard using channel 0
  Cayenne.celsiusWrite(1, t);     // To write the Sensor reading to Cayenne Dashboard using channel 1
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  // Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");  
}
