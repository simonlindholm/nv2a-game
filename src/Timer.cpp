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
	: simpleTimer(), active(false), paused(false)
{}

Timer::Timer(int left)
	: simpleTimer(left), active(true), paused(false)
{}

void Timer::step(unsigned int delay) {
	if (active && !paused) simpleTimer.step(delay);
}

void Timer::changeTime(int ms) {
	if (active) {
		if (ms >= 0)
			simpleTimer.addTime(ms);
		else
			simpleTimer.step(-ms);
	}
}

void Timer::set(int left) {
	simpleTimer.setTimer(left);
	active = true;
	paused = false;
}

void Timer::stop() {
	active = false;
	paused = false;
}

bool Timer::isActive() const {
	return active;
}

bool Timer::isDone() const {
	return (active && !paused && simpleTimer.isDone());
}

void Timer::pause() {
	if (active)
		paused = true;
}

void Timer::unpause() {
	if (active)
		paused = false;
}

bool Timer::isPaused() const {
	return (active && paused);
}
