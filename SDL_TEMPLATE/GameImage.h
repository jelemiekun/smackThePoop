#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "App_Info.h"

class GameImage {
public:
	SDL_Texture* mTexture;
	SDL_Rect* srcRect;

public:
	GameImage();
	~GameImage();

	void loadFromFile(SDL_Renderer*& renderer, const char* path);

	void render(SDL_Renderer*& renderer, const SDL_Rect* dstRect) const;

	SDL_Rect* getSrcRect() const;

	void close() const;
};