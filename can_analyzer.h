#ifndef CAN_ANALYZER_H
#define CAN_ANALYZER_H

// Vehicle control buttons
#define START_BUTTON 18
#define ACCEL_BUTTON 19
#define BRAKE_BUTTON 21

// LEDs
#define GREEN_LED 25
#define RED_LED 26
#define YELLOW_LED 27

// OLED I2C
#define OLED_SDA 22
#define OLED_SCL 23

struct CANFrame
{
    const char* can_id;
    float value;
    const char* unit;
};

void initializeCANAnalyzer();

void updateVehicle(float dt);

void getVehicleFrames(CANFrame frames[]);

#endif