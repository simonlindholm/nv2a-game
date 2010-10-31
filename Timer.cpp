#include "Timer.h"

SimpleTimer::SimpleTimer()
	: left(0)
{}

SimpleTimer::SimpleTimer(int left)
	: left(left)
{}

void SimpleTimer::step(unsigned int delay) {
	left -= delay;
}

void SimpleTimer::addTime(unsigned int ms) {
	left += ms;
}

void SimpleTimer::setTimer(int left) {
	this->left = left;
}

bool SimpleTimer::isDone() const {
	return left <= 0;
}

Timer::Timer()
	: timer(), running(false)
{}

Timer::Timer(int left)
	: timer(left), running(true)
{}

void Timer::step(unsigned int delay) {
	if (running) simpleTimer.step(delay);
}

void Timer::changeTime(int ms) {
	if (ms >= 0)
		simpleTimer.addTime(ms);
	else
		simpleTimer.step(-ms);
}

void Timer::set(int left) {
	simpleTimer.setTimer(left);
	running = true;
}

bool Timer::isDone() const {
	return (running && simpleTimer.isDone());
}

void Timer::pause() {
	running = false;
}

void Timer::unpause() {
	running = true;
}

bool Timer::isPaused() const {
	return !running;
}
