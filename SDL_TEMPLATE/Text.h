#pragma once
#include "SDL_ttf.h"
#include "GameImage.h"
#include <iostream>

enum FONT_SIZES {
	SIZE_TIMER = 28, 
	SIZE_GAME = 34,
	SIZE_PLAY_AGAIN = 16
};

class Text : public GameImage {
private:
	TTF_Font* gFont;

public:
	Text();
	~Text();

	void setGFont(TTF_Font* font);

	void loadFromRenderedText(SDL_Renderer* renderer, std::string text, SDL_Color color, SDL_Rect* dstRect);

	void close();
};

