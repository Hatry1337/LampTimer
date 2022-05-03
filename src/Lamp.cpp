#include "Lamp.h"

Lamp::Lamp(uint8_t lampPin, uint8_t lampOnLevel) {
    this->lampPin = lampPin;
    this->lampOnLevel = lampOnLevel;
    this->state = false;
}

void Lamp::begin(){
    pinMode(this->lampPin, OUTPUT_OPEN_DRAIN);
	digitalWrite(this->lampPin, !this->lampOnLevel);
    this->state = false;
}

void Lamp::setTimer(uint64_t time) {
    if(this->timer != 0) return;
    this->timer = millis() + time;
    digitalWrite(this->lampPin, this->lampOnLevel);
    this->state = true;
}

void Lamp::clearTimer() {
    if(this->timer == 0) return;
    this->timer = 0;
    digitalWrite(this->lampPin, !this->lampOnLevel);
    this->state = false;
}

void Lamp::turnOn() {
    if(this->timer != 0) return;
    digitalWrite(this->lampPin, this->lampOnLevel);
    this->state = true;
}

void Lamp::turnOff() {
    if(this->timer != 0) return;
    digitalWrite(this->lampPin, !this->lampOnLevel);
    this->state = false;
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

bool Lamp::getState() {
    return this->state;
}

void Lamp::think() {
    if (millis() > this->timer) {
        this->clearTimer();
    }
}