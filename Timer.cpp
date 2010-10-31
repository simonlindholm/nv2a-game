#pragma once
#include "Timer.h"

Timer::~Timer() {}

Timer::Timer(int t) {
	started = false;
	paused = false;
	startTime = 0;
	pausedTime = 0;
	waitingTime = t;
}

void Timer::start() {
	started = true;
	paused = false;
	startTime = SDL_GetTicks();
}

void Timer::stop() {
	started = false;
	paused = false;
	startTime = 0;
}

bool Timer::isStarted() const {
	return started;
}

void Timer::prolong(int delay) {
	waitingTime += delay;
}

void Timer::advance(int adv) {
	waitingTime -= adv;
}

void Timer::setWaitingTime(int newTime) {
	waitingTime = newTime;
}

void Timer::setPause(bool p) {
	if( p )
		pausedTime = getTicks();
	else
		startTime = SDL_GetTicks() - pausedTime;
	
	paused = p;	
}

bool Timer::isPaused() const {
	return paused;
}

bool Timer::isDone() const {
	if(waitingTime - getTicks() < 0)
	{
		return true;
	}

	return false;
}

int Timer::getTicks() const {
	if(started)
	{
		if(paused)
			return pausedTime;
		else
			return (SDL_GetTicks() - startTime);
	}

	return 0;
}