#pragma once
#include "SDL.h"

constexpr const char* SCREEN_TITLE = "Save The Poop";
constexpr int SCREEN_X_POS = SDL_WINDOWPOS_CENTERED;
constexpr int SCREEN_Y_POS = SDL_WINDOWPOS_CENTERED;
constexpr Uint16 SCREEN_WIDTH = 640;
constexpr Uint16 SCREEN_HEIGHT = 640;
constexpr Uint32 SCREEN_FLAGS = SDL_WINDOW_SHOWN;

constexpr Uint16 MAX_FPS = 60;
constexpr float FRAME_DURATION = 1000.0F / MAX_FPS;

constexpr int FONT_SIZE = 28;