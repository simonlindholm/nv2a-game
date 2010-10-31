#pragma once

class SimpleTimer {
	private:
		int left;
	public:
		SimpleTimer();
		SimpleTimer(int left);
		void step(unsigned int delay);
		void addTime(unsigned int ms);
		void setTimer(int left);
		bool isDone() const;
};

class Timer {
	private:
		SimpleTimer simpleTimer;
		bool running;
	public:
		Timer(int left);
		Timer();
		void step(unsigned int delay);
		void changeTime(int ms);
		void set(int left);
		bool isDone() const;
		void pause();
		void unpause();
		bool isPaused() const;
};
