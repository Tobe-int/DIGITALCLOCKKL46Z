// This is an example codes of digital clock on a four 7-seg LED display
// This code implements a digital clock on a four 7-segment LED display 
//  using an SN74HC595N shift register on a KL46Z development board.
//  Here's a breakdown of how it works:


#include "mbed.h"
#include "ShiftOut.h"

Serial pc(USBTX, USBRX, 9600);  
ShiftOut reg(D8, D7, D4, D10, D11); 
Timer rtc_timer;
Ticker blinkTicker;

// Time variables
int hours = 23;
int minutes = 59;
int seconds = 59;

// Segment patterns for digits 0–9
const uint8_t dts[11] = {
    0x03, 0x9F, 0x25, 0x0D, 0x99,
    0x49, 0x41, 0x1F, 0x01, 0x09, 0xFF
};

const uint8_t dtd[10] = {
    0x02, 0x9E, 0x24, 0x0C, 0x98,
    0x48, 0x40, 0x1E, 0x00, 0x08
};

// Switches
DigitalIn sw1(A1);  // Field toggle (e.g., HH ↔ MM or MM ↔ SS)
DigitalIn sw2(A3);  // Setup mode (hold)
DigitalIn sw3(A2);  // Increment selected field

// Flags
bool isHHMM = true;          // Display mode
bool inSetup = false;        // Setup mode active
bool settingFirst = true;    // Which field: true = HH or MM, false = MM or SS
bool blinkState = true;      // For blinking digits

void toggleBlink() {
    blinkState = !blinkState;
}

void displayTime() {
    if (isHHMM) {
        // HH:MM display
        // Minutes (right two digits)
        reg.writeByte((!(inSetup && !settingFirst && !blinkState) ? dts[minutes % 10] : 0xFF));
        reg.writeByte(0x10);
        wait_us(3000);

        reg.writeByte((!(inSetup && !settingFirst && !blinkState) ? (dts[minutes / 10]) : 0xFF));
        reg.writeByte(0x20);
        wait_us(3000);

        // Hours (left two digits)
        reg.writeByte((!(inSetup && settingFirst && !blinkState) ? dtd[hours % 10] : 0xFF));
        reg.writeByte(0x40);
        wait_us(3000);

        reg.writeByte((!(inSetup && settingFirst && !blinkState) ? dts[hours / 10] : 0xFF));
        reg.writeByte(0x80);
        wait_us(3000);
    } else {
        // MM:SS display
        // Seconds (right two digits)
        reg.writeByte((!(inSetup && !settingFirst && !blinkState) ? dts[seconds % 10] : 0xFF));
        reg.writeByte(0x10);
        wait_us(3000);

        reg.writeByte((!(inSetup && !settingFirst && !blinkState) ? (dts[seconds / 10]) : 0xFF));
        reg.writeByte(0x20);
        wait_us(3000);

        // Minutes (left two digits)
        reg.writeByte((!(inSetup && settingFirst && !blinkState) ? dtd[minutes % 10] : 0xFF));
        reg.writeByte(0x40);
        wait_us(3000);

        reg.writeByte((!(inSetup && settingFirst && !blinkState) ? dts[minutes / 10] : 0xFF));
        reg.writeByte(0x80);
        wait_us(3000);
    }
}

int main() {
    rtc_timer.start();
    blinkTicker.attach(&toggleBlink, 0.5); // blink every 0.5s

    bool prevSW1 = sw1.read();
    bool prevSW3 = sw3.read();

    while (1) {
        inSetup = (sw2.read() == 0);

        // Update clock if not in setup
        if (!inSetup && rtc_timer.read() >= 1.0) {
            rtc_timer.reset();
            seconds++;

            if (seconds >= 60) {
                seconds = 0;
                minutes++;

                if (minutes >= 60) {
                    minutes = 0;
                    hours = (hours + 1) % 24;
                }
            }
        }

        if (inSetup) {
            // Toggle field (HH ↔ MM or MM ↔ SS)
            bool curSW1 = sw1.read();
            if (prevSW1 == 1 && curSW1 == 0) {
                settingFirst = !settingFirst;
                wait_us(200000);
            }
            prevSW1 = curSW1;

            // Increment selected field
            bool curSW3 = sw3.read();
            if (prevSW3 == 1 && curSW3 == 0) {
                if (isHHMM) {
                    if (settingFirst)
                        hours = (hours + 1) % 24;
                    else
                        minutes = (minutes + 1) % 60;
                } else {
                    if (settingFirst)
                        minutes = (minutes + 1) % 60;
                    else
                        seconds = (seconds + 1) % 60;
                }
                wait_us(200000);
            }
            prevSW3 = curSW3;

        } else {
            // Normal mode: toggle HHMM <-> MMSS
            bool curSW1 = sw1.read();
            if (prevSW1 == 1 && curSW1 == 0) {
                isHHMM = !isHHMM;
                wait_us(200000);
            }
            prevSW1 = curSW1;
        }

        displayTime();
    }
}
