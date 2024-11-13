#pragma once
#include "GameImage.h"
class Heart : public GameImage {
public:
	Heart();
	~Heart();

	void render(SDL_Renderer*& renderer, const SDL_Rect* dstRect, const bool &isRed);
};

