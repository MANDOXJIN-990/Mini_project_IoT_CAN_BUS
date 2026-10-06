#include <Arduino.h>
#include <WiFi.h>

#include "can_analyzer.h"
#include "mqtt_manager.h"

// ===============================
// WIFI CONFIGURATION
// ===============================

const char* WIFI_SSID = "Wokwi-GUEST";

const char* WIFI_PASSWORD = "";

// ===============================
// TIMERS
// ===============================

unsigned long lastUpdate = 0;

unsigned long lastPublish = 0;

// ===============================
// WIFI CONNECTION
// ===============================

void connectWiFi()
{
    Serial.println();

    Serial.println("Connecting to WiFi...");

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");
    }

    Serial.println();

    Serial.println("WiFi connected!");

    Serial.print("IP Address: ");

    Serial.println(
        WiFi.localIP()
    );
}

// ===============================
// SETUP
// ===============================

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();

    Serial.println("==============================");

    Serial.println(" WIRELESS CAN BUS ANALYZER");

    Serial.println("==============================");

    Serial.println();

    // Initialize vehicle system
    initializeCANAnalyzer();

    // Connect WiFi
    connectWiFi();

    // Initialize MQTT
    setupMQTT();

    // Initialize timers
    lastUpdate = millis();

    lastPublish = millis();

    Serial.println();

    Serial.println("System ready.");

    Serial.println();
}

// ===============================
// MAIN LOOP
// ===============================

void loop()
{
    // Check WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    // MQTT processing
    mqttLoop();

    // Current time
    unsigned long currentTime = millis();

    // Calculate time difference
    float dt =
        (currentTime - lastUpdate)
        / 1000.0;

    lastUpdate = currentTime;

    // Update vehicle simulation
    updateVehicle(dt);

    // Publish every 1 second
    if (currentTime - lastPublish >= 1000)
    {
        lastPublish = currentTime;

        publishVehicleData();

        Serial.println(
            "Vehicle data published"
        );
    }

    delay(20);
}