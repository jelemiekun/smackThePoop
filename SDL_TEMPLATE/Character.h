#pragma once
#include "GameImage.h"
class Character : public GameImage {
private:
	SDL_Rect gSpriteClips[2];
	SDL_Rect rectCharacter;
	Uint8 frameCount;
public:
	Character();
	~Character();
	
	void init();
	void animate(SDL_Renderer* rendere);
};

