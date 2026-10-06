#include "can_generator.h"

CANFrame generateCANFrame(int index) {

    CANFrame frame = {};

    frame.dlc = 8;
    frame.timestamp = millis();

    switch (index % 5) {

        case 0:
            frame.id = 0x100;
            frame.data[0] = 0x09;
            frame.data[1] = 0xC4;
            break;

        case 1:
            frame.id = 0x101;
            frame.data[0] = 0x00;
            frame.data[1] = 0x3C;
            break;

        case 2:
            frame.id = 0x102;
            frame.data[0] = 0x00;
            frame.data[1] = 0x55;
            break;

        case 3:
            frame.id = 0x103;
            frame.data[0] = 0x00;
            frame.data[1] = 0x41;
            break;

        case 4:
            frame.id = 0x104;
            frame.data[0] = 0x04;
            frame.data[1] = 0xE2;
            break;
    }

    return frame;
}