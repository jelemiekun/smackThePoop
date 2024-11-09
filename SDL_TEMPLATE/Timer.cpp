#include "Timer.h"

Timer::Timer() : startingTime(0), currentTime(0), finish(true), stopped(false) {}

Timer::~Timer() {}

void Timer::setStartingTime(Uint32 time) {
	startingTime = time;
}

void Timer::startTimer() {
	if (finish) {
		currentTime = SDL_GetTicks();
		finish = false;
		remainingTime = 0;
	}
}

Uint32 Timer::getRawTime() {
	if (stopped)
		return remainingTime;

	if (!finish) {
		Uint32 timeDiff = SDL_GetTicks() - currentTime;

		if (timeDiff >= startingTime) {
			finish = true;
			return 0;
		}

		Uint32 timeRemaining = startingTime - timeDiff;

		return timeRemaining;
	} else {
		return 0;
	}
}

std::string Timer::getTimeInFormat() {
	Uint32 timeRemaining = getRawTime();

	Uint32 secondsRemaining = timeRemaining / 1000;

	Uint32 minutes = secondsRemaining / 60;
	Uint32 seconds = secondsRemaining % 60;

	std::ostringstream timeStream;
	timeStream << std::setw(2) << std::setfill('0') << minutes << ":"
		<< std::setw(2) << std::setfill('0') << seconds;

	return timeStream.str();
}

bool Timer::isFinish() const {
	return finish;
}

void Timer::stopTimer() {
	if (!finish && !stopped) {
		Uint32 timeDiff = SDL_GetTicks() - currentTime;
		remainingTime = startingTime > timeDiff ? startingTime - timeDiff : 0; 
		stopped = true; 
	}
}