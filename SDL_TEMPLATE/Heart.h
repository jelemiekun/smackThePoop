#pragma once
#include "GameImage.h"
class Heart : public GameImage {
public:
	Heart();
	~Heart();

	void render(SDL_Renderer* renderer, SDL_Rect* dstRect, bool isRed);
};

