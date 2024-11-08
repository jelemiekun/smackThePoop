#pragma once
#include "SDL.h"

namespace GameFPS {
	static Uint32 startFrame;
	static Uint32 startTime;
	static int frameCount;

	void initCalculateFPS();
	void calculateFPS();
	void initLimitFPS();
	void limitFPS();
};

