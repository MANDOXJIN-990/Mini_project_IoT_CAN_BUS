# Mini_project_IoT_CAN_BUS
# Wireless CAN Bus Analyzer using ESP32, MQTT and Node-RED

## Overview

An IoT-based Wireless CAN Bus Analyzer developed using ESP32 and Wokwi. The system simulates automotive parameters such as RPM, vehicle speed, temperature, fuel level, and battery voltage, then transmits the data wirelessly using MQTT.

The received data is processed in Node-RED and displayed through a real-time dashboard with gauges, live graphs, vehicle status, and CAN statistics. An SSD1306 OLED provides local parameter monitoring, while LEDs indicate engine and warning conditions.

## Technologies Used

- ESP32
- PlatformIO
- Wokwi
- MQTT
- HiveMQ
- Node-RED
- SSD1306 OLED
- C/C++
- JavaScript

## System Flow

START/STOP + ACCEL + BRAKE  
↓  
ESP32 Vehicle Simulation  
↓  
Simulated CAN Data  
↓  
Wi-Fi + MQTT  
↓  
HiveMQ MQTT Broker  
↓  
Node-RED  
↓  
Real-Time Dashboard

## Simulated CAN Data

| CAN ID | Parameter |
|--------|-----------|
| 0x100 | RPM |
| 0x101 | Vehicle Speed |
| 0x102 | Temperature |
| 0x103 | Fuel Level |
| 0x104 | Battery Voltage |

## Features

- Vehicle START/STOP control
- ACCEL and BRAKE simulation
- Real-time vehicle parameter generation
- OLED parameter display
- Engine ON/OFF LED indication
- High-temperature warning LED
- Wireless MQTT communication
- Node-RED dashboard
- Real-time gauges and graphs
- CAN statistics

## MQTT Configuration

**Broker:** `broker.hivemq.com`

**Port:** `1883`

**Topic:** `ishan/canbus/data`

## Project Structure

```text
Wireless-CAN-Bus-Analyzer/
├── platformio.ini
├── diagram.json
├── README.md
├── include/
│   ├── can_analyzer.h
│   └── mqtt_manager.h
└── src/
    ├── main.cpp
    ├── can_analyzer.cpp
    └── mqtt_manager.cpp
