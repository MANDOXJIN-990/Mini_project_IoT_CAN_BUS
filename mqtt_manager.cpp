#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "mqtt_manager.h"
#include "can_analyzer.h"

// ===============================
// MQTT CONFIGURATION
// ===============================

const char* MQTT_SERVER = "broker.hivemq.com";

const int MQTT_PORT = 1883;

const char* MQTT_TOPIC = "ishan/canbus/data";

// ===============================
// MQTT CLIENT
// ===============================

WiFiClient espClient;

PubSubClient mqttClient(espClient);

// ===============================
// CONNECT TO MQTT
// ===============================

void connectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT...");

        String clientId = "ESP32-CAN-Analyzer-";

        clientId += String(random(0xffff), HEX);

        if (mqttClient.connect(clientId.c_str()))
        {
            Serial.println("MQTT connected!");
        }
        else
        {
            Serial.print("MQTT connection failed. State: ");

            Serial.println(
                mqttClient.state()
            );

            delay(2000);
        }
    }
}

// ===============================
// MQTT SETUP
// ===============================

void setupMQTT()
{
    mqttClient.setServer(
        MQTT_SERVER,
        MQTT_PORT
    );

    connectMQTT();
}

// ===============================
// MQTT LOOP
// ===============================

void mqttLoop()
{
    if (!mqttClient.connected())
    {
        connectMQTT();
    }

    mqttClient.loop();
}

// ===============================
// PUBLISH CAN DATA
// ===============================

void publishVehicleData()
{
    CANFrame frames[5];

    getVehicleFrames(frames);

    for (int i = 0; i < 5; i++)
    {
        String payload = "{";

        payload += "\"can_id\":\"";

        payload += frames[i].can_id;

        payload += "\",";

        payload += "\"value\":";

        payload += String(
            frames[i].value,
            2
        );

        payload += ",";

        payload += "\"unit\":\"";

        payload += frames[i].unit;

        payload += "\"";

        payload += "}";

        mqttClient.publish(
            MQTT_TOPIC,
            payload.c_str()
        );

        Serial.print("CAN FRAME -> ");

        Serial.println(payload);

        delay(20);
    }
}