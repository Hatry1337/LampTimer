#include "Arduino.h"

class Lamp {
	private:
		uint8_t lampPin;
		uint8_t lampOnLevel;
		uint64_t timer;

	public:
		Lamp(uint8_t lampPin, uint8_t lampOnLevel);
		void turnOn();
		void turnOff();
		void setTimer(uint64_t time);
		void clearTimer();
		uint64_t getTimeRemaining();

		void think();
};