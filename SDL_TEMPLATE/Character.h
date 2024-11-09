#pragma once
#include "GameImage.h"
class Character : public GameImage {
private:
	SDL_Rect gSpriteClips[2];
	SDL_Rect rectCharacter;
	Uint8 frameCount;
	static constexpr int DMG_FRAME_COUNT = 20;
	bool takingDmg;

private:
	void takeDamage();

public:
	Character();
	~Character();
	
	void init();
	void animate(SDL_Renderer* renderer, bool takeDamage);
};

