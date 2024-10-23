#include <ArduinoHttpClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;  // Number of temperature devices found

DeviceAddress tempDeviceAddress;  // We'll use this variable to store a found device address

const char *ssid = "";    // The name of the Wi-Fi network that will be created
const char *password = "";  // The password required to connect to it, leave blank for an open network

const char *serverUrl = "";  // Local server url

unsigned long previousMillis = 0;
const long interval = 30000;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Start up the library
  sensors.begin();

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Print the MAC address of the NodeMCU
  Serial.print("NodeMCU MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Print the IP address assigned to the NodeMCU
  Serial.print("NodeMCU IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();  // Get current time

  sensors.requestTemperatures();  // Send the command to get temperatures

  WiFiClient wifiClient;
  HTTPClient http;

  // Check if the interval has passed
  if (currentMillis - previousMillis >= interval) {
    /// ====================

    // Loop through each device, print out temperature data
    for (int i = 0; i < numberOfDevices; i++) {
      // Search the wire for address
      if (sensors.getAddress(tempDeviceAddress, i)) {

        // Output the device ID
        Serial.print("Temperature for device: ");
        Serial.println(i, DEC);

        // Print the data
        float tempC = sensors.getTempC(tempDeviceAddress);
        Serial.print("Temp C: ");
        Serial.print(tempC);

        // http request logic
        http.begin(wifiClient, serverUrl);                   // Specify the URL for the POST request
        http.addHeader("Content-Type", "application/json");  // Set the content type

        String postData = "{\"temperature\": " + String(tempC, 2) + ", \"name\": \"sensor_"+String(i)+"\" }";
        Serial.print(postData);
        // Send the POST request
        int httpResponseCode = http.POST(postData);

        // Check the response
        if (httpResponseCode > 0) {
          String response = http.getString();  // Get the response payload
          Serial.println(httpResponseCode);    // Print HTTP response code
          Serial.println(response);            // Print response from server
        } else {
          Serial.print("Error on sending POST: ");
          Serial.println(httpResponseCode);
        }

        http.end();  // End the connection
      }
    }
    /// ====================
    Serial.println("Interval passed, updating sensor data.");

    previousMillis = currentMillis;  // Update the last execution time

  }

  yield();  // Feed the watchdog timer to prevent resets
}
