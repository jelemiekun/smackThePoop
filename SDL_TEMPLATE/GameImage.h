#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class GameImage {
public:
	SDL_Texture* mTexture;
	SDL_Rect* srcRect;

public:
	GameImage();
	~GameImage();

	void loadFromFile(SDL_Renderer* renderer, const char* path);


	void render(SDL_Renderer* renderer, SDL_Rect* dstRect);

	SDL_Rect* getSrcRect();
};

