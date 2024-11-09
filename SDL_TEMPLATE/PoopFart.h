#pragma once
#include "GameImage.h"

struct ToRelease {
	unsigned int state : 3;
};

class PoopFart : public GameImage {
private:
	static constexpr int ANIMATION_PARTS = 6;
	static constexpr int FRAMES_PER_ANIMATION = 8;
	static constexpr int EACH_FRAME_DURATION = 6;
	int frameCount;
	bool finishRendering;
	int widthEachSprite;
	int heightEachSprite;

	SDL_Rect* rectArrays[ANIMATION_PARTS][FRAMES_PER_ANIMATION];
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
	void render(SDL_Renderer* renderer);
	void close();
};

