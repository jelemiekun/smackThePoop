#pragma once
#include "SDL_ttf.h"
#include "GameImage.h"
#include <iostream>

class Text : public GameImage {
private:
	TTF_Font* gFont;

public:
	Text();
	~Text();

	void setGFont(TTF_Font* font);

	void loadFromRenderedText(SDL_Renderer* renderer, std::string text, SDL_Color color, SDL_Rect* dstRect);

};

