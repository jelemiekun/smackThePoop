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
	bool finish;

public:
	Timer();
	~Timer();

	void setStartingTime(Uint32 time);

	void startTimer();

	Uint32 getRawTime();

	std::string getTimeInFormat();

	bool isFinish() const;
};

