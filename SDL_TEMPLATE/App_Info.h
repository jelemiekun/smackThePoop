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

constexpr int FONT_SIZE_TIMER = 28;
constexpr int FONT_SIZE_GAME = 34;
constexpr int FONT_SIZE_PLAY_AGAIN = 16;

constexpr auto PRIMARY_BUTTON = SDLK_RETURN;
constexpr auto LEFT_BUTTON = SDLK_j;
constexpr auto RIGHT_BUTTON = SDLK_k;

constexpr bool LIMIT = true;

constexpr int ALLOWANCE_TIME_TO_PLAY_POOP_END = 500;
constexpr int ALLOWANCE_TIME_TO_CLICK_PLAYER = 500;
constexpr int ALLOWANCE_TIME_GAMEPLAY = 120000;

constexpr int DMG_FRAME_COUNT = 20;

constexpr int POOP_COUNT = 15;

constexpr int POOPFART_ANIMATION_PARTS = 6;
constexpr int POOPFART_FRAMES_PER_ANIMATION = 8;
constexpr int POOPFART_EACH_FRAME_DURATION = 6;