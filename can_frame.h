#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <Arduino.h>

struct CANFrame {
    uint16_t id;
    uint8_t dlc;
    uint8_t data[8];
    unsigned long timestamp;
};

struct DecodedCAN {
    const char* parameter;
    const char* unit;
    float value;
};

#endif // CAN_FRAME_H