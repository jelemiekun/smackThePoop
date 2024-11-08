#include "GameImage.h"

GameImage::GameImage() : mTexture(nullptr), srcRect(new SDL_Rect) {}

GameImage::~GameImage() {}

void GameImage::loadFromFile(SDL_Renderer* renderer, const char* path) {
	SDL_Surface* newSurface = IMG_Load(path);

	if (newSurface == nullptr) {
		std::cout << "Failed to load image path: " << IMG_GetError() << '\n';
		return;
	}

	mTexture = SDL_CreateTextureFromSurface(renderer, newSurface);

	if (mTexture == nullptr) {
		std::cout << "Failed to create texture from surface: " << IMG_GetError() << '\n';
		return;
	} else {
		std::cout << "Image " << path << " loaded." << '\n';

		srcRect->x = 0;
		srcRect->y = 0;
		SDL_QueryTexture(mTexture, NULL, NULL, &srcRect->w, &srcRect->h);
	}

	SDL_FreeSurface(newSurface);
}

void GameImage::render(SDL_Renderer* renderer, SDL_Rect* dstRect) {
	SDL_RenderCopy(renderer, mTexture, srcRect, dstRect);
}

SDL_Rect* GameImage::getSrcRect() {
	return srcRect;
}

void GameImage::close() {
	SDL_DestroyTexture(mTexture);
}