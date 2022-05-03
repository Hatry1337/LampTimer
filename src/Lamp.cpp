#include "Lamp.h"

Lamp::Lamp(uint8_t lampPin, uint8_t lampOnLevel) {
    this->lampPin = lampPin;
    this->lampOnLevel = lampOnLevel;
    pinMode(lampPin, OUTPUT);
	digitalWrite(lampPin, !lampOnLevel);
}

void Lamp::setTimer(uint64_t time) {
    if(this->timer != 0) return;
    this->timer = millis() + time;
    digitalWrite(this->lampPin, this->lampOnLevel);
}

void Lamp::clearTimer() {
    if(this->timer == 0) return;
    this->timer = 0;
    digitalWrite(this->lampPin, !this->lampOnLevel);
}

void Lamp::turnOn() {
    if(this->timer != 0) return;
    digitalWrite(this->lampPin, this->lampOnLevel);
}

void Lamp::turnOff() {
    if(this->timer != 0) return;
    digitalWrite(this->lampPin, !this->lampOnLevel);
}

uint64_t Lamp::getTimeRemaining() {
    if(this->timer == 0) return 0;
    uint64_t mils = millis();
    if(mils > this->timer) {
        this->clearTimer();
        return 0;
    }
    return this->timer - millis();
}

void Lamp::think() {
    if (millis() > this->timer) {
        this->clearTimer();
    }
}