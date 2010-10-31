#pragma once
#include <SDL/SDL.h>

class Timer {
	private:
		int waitingTime;
		int startTime;
		int pausedTime;
		bool paused;
		bool started;

	public:
		~Timer();

		Timer(int t);
		
		//Start timer
		void start();

		//Stop timer
		void stop();

		//Check if timer is started
		bool isStarted() const;

		//Delay timer
		void prolong(int delay);

		//Advance timer
		void advance(int adv);

		//Set the timer
		void setWaitingTime(int newTime);

		//Pause or unpause timer
		void setPause(bool p);

		//Check if timer is paused
		bool isPaused() const;

		//Check if timer is done
		bool isDone() const;

		//Get the time
		int getTicks() const;
};