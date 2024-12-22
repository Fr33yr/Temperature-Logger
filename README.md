### ESP8266 Temperature Logger

## Overview

This project uses an ESP8266 microcontroller to monitor temperature readings from DS18B20 sensors and send the data to a server via HTTP POST requests. The project leverages the Arduino framework and key libraries for Wi-Fi connectivity and temperature sensing.

## Features

- Connects to Wi-Fi and sends temperature data to a specified server.

- Supports multiple DS18B20 sensors on a single data line.

- Periodic data updates at a specified interval (default: 30 seconds).

- Provides debugging information via serial communication.


## Components

- **ESP8266 NodeMCU** – Microcontroller

- **DS18B20** – Digital temperature sensors

- **4.7kΩ Resistor** – Pull-up resistor for the DS18B20 data line


## Required Libraries

 Install the following libraries through the Arduino Library Manager:

- `ArduinoHttpClient`

- `OneWire`

- `DallasTemperature`

- `ESP8266WiFi`

- `ESP8266HTTPClient`

## Circuit Diagram

- ONE_WIRE_BUS (GPIO 4): Connect DS18B20 data pin to GPIO 4 (D2 on NodeMCU)

- Power (VCC): Connect to 3.3V (or 5V based on sensor)

- Ground (GND): Connect to GND

- 4.7kΩ Resistor: Connect between the data pin and VCC
