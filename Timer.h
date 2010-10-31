#pragma once

// A simplistic timer class, only capable of counting downwards, changing the
// time, and checking if it has fired. Note that the time left can become
// negative; the timer does not stop after firing.
class SimpleTimer {
	private:
		int left;
	public:
		// Create a timer that fires immediately
		SimpleTimer();

		// Create a timer that fires in 'left' milliseconds
		SimpleTimer(int left);

		// Step the internal clock by 'delay' ms
		void step(unsigned int delay);

		// The opposite of 'step'; increase the time left by 'ms'
		void addTime(unsigned int ms);

		// Reset the timer to 'left' ms left, like the constructor
		void setTimer(int left);

		// Check if the timer has fired
		bool isDone() const;
};

// A more advanced timer class, which acts like SimpleTimer but also supports
// pausing and stopping the timer.
class Timer {
	private:
		SimpleTimer simpleTimer;
		bool active, paused;
	public:
		// Create a new stopped timer
		Timer();

		// Create a new (active) timer with 'left' ms left until firing
		Timer(int left);

		// Step the internal clock by 'delay' ms, unless the timer is paused
		// or not running
		void step(unsigned int delay);

		// Change the time left until fire by 'ms' milliseconds; a positive 
		// value increases the duration, and a negative value decreases it
		// (only applicable for active timers)
		void changeTime(int ms);

		// Set the timer to active mode, with 'left' ms until firing
		void set(int left);

		// Stop the timer
		void stop();

		// Check whether the timer is active
		bool isActive() const;

		// Check if the timer is firing (this cannot happen when paused
		// or stopped)
		bool isDone() const;

		// Pause the timer (does nothing if the timer is already paused)
		void pause();

		// Start the timer again, with the same duration until fire as before
		// it was paused
		void unpause();

		// Check whether the timer is currently paused
		bool isPaused() const;
};
