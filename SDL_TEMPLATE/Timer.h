#pragma once
#include "SDL.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

class Timer {
private:
	Uint32 startingTime;
	Uint32 currentTime;
	Uint32 remainingTime;
	bool finish;
	bool stopped;

public:
	Timer();
	~Timer();

	void setStartingTime(const Uint32 &time);
	void startTimer();
	Uint32 getRawTime();
	std::string getTimeInFormat();
	bool isFinish() const;
	void stopTimer();
	void resetTimer();
};

