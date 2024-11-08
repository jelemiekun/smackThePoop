#include "GameFPS.h"
#include "App_Info.h"
#include "iostream"

void GameFPS::initCalculateFPS() {
	startTime = SDL_GetTicks();
	frameCount = 0;
}

void GameFPS::calculateFPS() {
    frameCount++;
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - startTime > 1000) {
        float fps = frameCount / ((currentTime - startTime) / 1000.0F);
        std::cout << "FPS: " << fps << '\n';
        startTime = currentTime;
        frameCount = 0;
    }
}

void GameFPS::initLimitFPS() {
    startFrame = SDL_GetTicks();
}

void GameFPS::limitFPS() {
    Uint32 frameDuration = SDL_GetTicks() - startFrame;

    if (frameDuration < FRAME_DURATION) {
        SDL_Delay(FRAME_DURATION - frameDuration);
    }
}