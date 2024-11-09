#include "Heart.h"

Heart::Heart() {}

Heart::~Heart() {}

void Heart::render(SDL_Renderer* renderer, SDL_Rect* dstRect, bool isRed) {
	getSrcRect()->x = isRed ? 0 : getSrcRect()->w;
	GameImage::render(renderer, dstRect);
}