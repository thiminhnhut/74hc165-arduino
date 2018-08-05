#include <Arduino.h>

#define __LATCH_PIN__ 5
#define __CLOCK_PIN__ 6
#define __DATA_PIN__ 7

#define __NUMBER_PULSE_CLOCK__ 8
#define __BAUD_RATE__ 9600

void init74HC165();
unsigned char getSignalButton();
unsigned char setBitIndex(unsigned char number, unsigned char index, bool bit);

void setup() {
    Serial.begin(__BAUD_RATE__);

    pinMode(__LATCH_PIN__, OUTPUT);
    pinMode(__CLOCK_PIN__, OUTPUT);
    pinMode(__DATA_PIN__, INPUT);

    init74HC165();

}

void loop() {
    unsigned char number = getSignalButton();
    if (number > 0) {
        Serial.println(number, BIN);
        delay(300);
    }
}

void init74HC165() {
    digitalWrite(__LATCH_PIN__, LOW);
    digitalWrite(__CLOCK_PIN__, LOW);
}

unsigned char getSignalButton() {
    digitalWrite(__LATCH_PIN__, LOW);
    delayMicroseconds(1);
    digitalWrite(__LATCH_PIN__, HIGH);
    delayMicroseconds(1);

    unsigned char number = 0x00;
    for (unsigned int i = 0; i < __NUMBER_PULSE_CLOCK__; i++) {
        number = setBitIndex(number, i, digitalRead(__DATA_PIN__));

        digitalWrite(__CLOCK_PIN__, LOW);
        delayMicroseconds(1);
        digitalWrite(__CLOCK_PIN__, HIGH);
        delayMicroseconds(1);
    }

    init74HC165();
    return number;
}

unsigned char setBitIndex(unsigned char number, unsigned char index, bool bit) {
    return number ^ ((-bit ^ number) & (1 << index));
}
