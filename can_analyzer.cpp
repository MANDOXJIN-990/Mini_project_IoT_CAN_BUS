#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "can_analyzer.h"

// ===============================
// OLED CONFIGURATION
// ===============================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// ===============================
// VEHICLE VARIABLES
// ===============================

static bool engineOn = false;

static float speed = 0.0;
static float rpm = 0.0;
static float temperature = 25.0;
static float fuel = 75.0;
static float battery = 12.6;

// ===============================
// BUTTON STATE
// ===============================

static bool lastStartButton = HIGH;

static unsigned long lastStartTime = 0;

// ===============================
// INITIALIZATION
// ===============================

void initializeCANAnalyzer()
{
    // Buttons
    pinMode(START_BUTTON, INPUT_PULLUP);
    pinMode(ACCEL_BUTTON, INPUT_PULLUP);
    pinMode(BRAKE_BUTTON, INPUT_PULLUP);

    // LEDs
    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);

    // Initial LED state
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);

    // OLED
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("OLED initialization failed!");
    }
    else
    {
        Serial.println("OLED initialized successfully.");

        display.clearDisplay();

        display.setTextColor(SSD1306_WHITE);

        display.setTextSize(1);

        display.setCursor(20, 15);
        display.println("WIRELESS CAN");

        display.setCursor(28, 30);
        display.println("BUS ANALYZER");

        display.setCursor(30, 48);
        display.println("SYSTEM READY");

        display.display();
    }

    Serial.println();
    Serial.println("CAN Analyzer initialized");
    Serial.println("START = GPIO18");
    Serial.println("ACCEL = GPIO19");
    Serial.println("BRAKE = GPIO21");
    Serial.println("GREEN LED = GPIO25");
    Serial.println("RED LED = GPIO26");
    Serial.println("YELLOW LED = GPIO27");
    Serial.println("OLED SDA = GPIO22");
    Serial.println("OLED SCL = GPIO23");
}

// ===============================
// VEHICLE UPDATE
// ===============================

void updateVehicle(float dt)
{
    // Prevent extremely large time step
    if (dt > 0.1)
        dt = 0.1;

    // ===============================
    // START / STOP BUTTON
    // ===============================

    bool startButton = digitalRead(START_BUTTON);

    // Detect button press
    if (
        lastStartButton == HIGH &&
        startButton == LOW &&
        millis() - lastStartTime > 300
    )
    {
        engineOn = !engineOn;

        lastStartTime = millis();

        if (engineOn)
        {
            Serial.println("ENGINE: ON");
        }
        else
        {
            Serial.println("ENGINE: OFF");
        }
    }

    lastStartButton = startButton;

    // ===============================
    // ACCELERATOR / BRAKE
    // ===============================

    bool accelerator =
        (digitalRead(ACCEL_BUTTON) == LOW);

    bool brake =
        (digitalRead(BRAKE_BUTTON) == LOW);

    // ===============================
    // SPEED
    // ===============================

    if (!engineOn)
    {
        // Vehicle slowly stops
        speed -= 15.0 * dt;
    }
    else if (accelerator && !brake)
    {
        // Accelerate
        speed += 35.0 * dt;
    }
    else if (brake)
    {
        // Brake
        speed -= 50.0 * dt;
    }
    else
    {
        // Natural deceleration
        speed -= 5.0 * dt;
    }

    // Speed limits
    if (speed < 0)
        speed = 0;

    if (speed > 180)
        speed = 180;

    // ===============================
    // RPM
    // ===============================

    if (engineOn)
    {
        rpm = 800.0 + (speed * 28.0);

        if (accelerator)
            rpm += 700.0;

        if (brake)
            rpm -= 300.0;

        if (rpm < 800)
            rpm = 800;

        if (rpm > 6000)
            rpm = 6000;
    }
    else
    {
        rpm = 0;
    }

    // ===============================
    // TEMPERATURE
    // ===============================

    if (engineOn)
    {
        temperature += 0.08 * dt;
    }
    else
    {
        temperature -= 0.12 * dt;
    }

    if (temperature < 25)
        temperature = 25;

    if (temperature > 110)
        temperature = 110;

    // ===============================
    // FUEL
    // ===============================

    if (engineOn)
    {
        if (accelerator)
        {
            fuel -= 0.02 * dt;
        }
        else
        {
            fuel -= 0.005 * dt;
        }
    }

    if (fuel < 0)
        fuel = 0;

    // ===============================
    // BATTERY
    // ===============================

    if (engineOn)
    {
        battery = 13.8;
    }
    else
    {
        battery = 12.6;
    }

    // ===============================
    // LED CONTROL
    // ===============================

    // Green LED = engine ON
    if (engineOn)
    {
        digitalWrite(GREEN_LED, HIGH);
    }
    else
    {
        digitalWrite(GREEN_LED, LOW);
    }

    // Red LED = engine OFF
    if (!engineOn)
    {
        digitalWrite(RED_LED, HIGH);
    }
    else
    {
        digitalWrite(RED_LED, LOW);
    }

    // Yellow LED = high temperature warning
    if (temperature >= 90)
    {
        digitalWrite(YELLOW_LED, HIGH);
    }
    else
    {
        digitalWrite(YELLOW_LED, LOW);
    }

    // ===============================
    // OLED DISPLAY
    // ===============================

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);

    // RPM
    display.setCursor(0, 0);
    display.print("RPM: ");
    display.println((int)rpm);

    // Speed
    display.setCursor(0, 11);
    display.print("SPD: ");
    display.print(speed, 1);
    display.println(" km/h");

    // Temperature
    display.setCursor(0, 22);
    display.print("TEMP: ");
    display.print(temperature, 1);
    display.println(" C");

    // Fuel
    display.setCursor(0, 33);
    display.print("FUEL: ");
    display.print(fuel, 1);
    display.println(" %");

    // Battery
    display.setCursor(0, 44);
    display.print("BATT: ");
    display.print(battery, 1);
    display.println(" V");

    // Vehicle status
    display.setCursor(0, 55);

    if (!engineOn)
    {
        display.print("ENGINE: OFF");
    }
    else if (brake)
    {
        display.print("BRAKING");
    }
    else if (accelerator)
    {
        display.print("ACCELERATING");
    }
    else
    {
        display.print("ENGINE: ON");
    }

    display.display();
}

// ===============================
// CAN FRAME GENERATION
// ===============================

void getVehicleFrames(CANFrame frames[])
{
    frames[0] =
    {
        "0x100",
        rpm,
        "RPM"
    };

    frames[1] =
    {
        "0x101",
        speed,
        "km/h"
    };

    frames[2] =
    {
        "0x102",
        temperature,
        "C"
    };

    frames[3] =
    {
        "0x103",
        fuel,
        "%"
    };

    frames[4] =
    {
        "0x104",
        battery,
        "V"
    };
}