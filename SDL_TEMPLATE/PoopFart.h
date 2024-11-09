#pragma once
#include "GameImage.h"

struct ToRelease {
	unsigned int state : 3;
};

class PoopFart : public GameImage {
private:
	int frameCount;
	bool finishRendering;
	int widthEachSprite;
	int heightEachSprite;

	SDL_Rect* rectArrays[POOPFART_ANIMATION_PARTS][POOPFART_FRAMES_PER_ANIMATION];
	SDL_Rect* dstRect;

	ToRelease* toRelease;

public:
	PoopFart();
	~PoopFart();

public:
	void init();
	void initSpriteSheet();
	unsigned int getToReleaseState();
	void generateRandomNumber();
	bool isFinishRendering() const;
	void setFinishRendering(bool finish);
	void setReleaseState(int state);
	void render(SDL_Renderer* renderer);
	void close();
};

