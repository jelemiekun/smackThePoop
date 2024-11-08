#pragma once
#include "GameImage.h"
class Character : public GameImage {
private:
	SDL_Rect gSpriteClips[2];
	Uint8 frameCount;
public:
	Character();
	~Character();
	
	void init();
	void animate(SDL_Renderer* rendere, SDL_Rect* dstRect);
};

