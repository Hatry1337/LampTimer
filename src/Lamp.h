#include "Arduino.h"

class Lamp {
	private:
		uint8_t lampPin;
		uint8_t lampOnLevel;
		uint64_t timer;
		bool state;

	public:
		Lamp(uint8_t lampPin, uint8_t lampOnLevel);
		void begin();
		void turnOn();
		void turnOff();
		bool getState();
		void setTimer(uint64_t time);
		void clearTimer();
		uint64_t getTimeRemaining();

		void think();
};