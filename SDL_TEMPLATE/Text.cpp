#include "Text.h"

Text::Text() : gFont(nullptr) {}

Text::~Text() {
	close();
}

void Text::setGFont(TTF_Font* font) {
	gFont = font;
}

void Text::loadFromRenderedText(SDL_Renderer* renderer, std::string text, SDL_Color color, SDL_Rect* dstRect) {
	SDL_Surface* newSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);

	if (newSurface == nullptr) {
		std::cout << "Unable to render text surface: " << TTF_GetError() << '\n';
		return;
	}

	mTexture = SDL_CreateTextureFromSurface(renderer, newSurface);

	if (mTexture == nullptr) {
		std::cout << "Failed to create texture from rendered text: " << SDL_GetError() << '\n';
		return;
	}

	SDL_QueryTexture(mTexture, NULL, NULL, &srcRect->w, &srcRect->h);
	srcRect->x = 0;
	srcRect->y = 0;

	SDL_FreeSurface(newSurface);

	GameImage::render(renderer, dstRect);
}

void Text::close() {
	if (gFont != nullptr) {
		TTF_CloseFont(gFont);
		gFont = nullptr;
	}
}