#include "Character.h"
#include "iostream"

static constexpr Uint8 CHARAC_FRAME_DURATION = 30;

Character::Character() : frameCount(0) {}

Character::~Character() {}

void Character::init() {
	gSpriteClips[0] = { 0, 0, getSrcRect()->w / 2, getSrcRect()->h };
	gSpriteClips[1] = { getSrcRect()->w / 2, 0, getSrcRect()->w / 2, getSrcRect()->h };
	std::cout << getSrcRect()->w / 2 << '\n';
}

void Character::animate(SDL_Renderer* renderer, SDL_Rect* dstRect) {
	srcRect = &gSpriteClips[frameCount / CHARAC_FRAME_DURATION];

	++frameCount;

	if (frameCount / CHARAC_FRAME_DURATION >= std::size(gSpriteClips))
		frameCount = 0;

	GameImage::render(renderer, dstRect);
}