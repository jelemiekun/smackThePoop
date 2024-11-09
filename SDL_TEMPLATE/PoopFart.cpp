#include "PoopFart.h"
#include <random>

PoopFart::PoopFart() : toRelease(nullptr), frameCount(0), finishRendering(true),
					widthEachSprite(0), heightEachSprite(0), dstRect(nullptr) {
	for (int i = 0; i < ANIMATION_PARTS; i++) {
		for (int j = 0; j < FRAMES_PER_ANIMATION; j++) {
			rectArrays[i][j] = nullptr;
		}
	}
}

PoopFart::~PoopFart() {}

unsigned int PoopFart::getRandomNumber() {
	static std::random_device dev;
	static std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, ANIMATION_PARTS - 1);
	return dist6(rng);
}

void PoopFart::init() {
	toRelease = new ToRelease;

	for (int i = 0; i < ANIMATION_PARTS; i++) {
		for (int j = 0; j < FRAMES_PER_ANIMATION; j++) {
			rectArrays[i][j] = new SDL_Rect;
		}
	}

	dstRect = new SDL_Rect;
}

void PoopFart::initSpriteSheet() {
	widthEachSprite = srcRect->w / FRAMES_PER_ANIMATION;
	heightEachSprite = srcRect->h / ANIMATION_PARTS;

	for (int i = 0; i < ANIMATION_PARTS; i++) {
		for (int j = 0; j < FRAMES_PER_ANIMATION; j++) {
			rectArrays[i][j]->x = widthEachSprite * j;
			rectArrays[i][j]->y = heightEachSprite * i;
			rectArrays[i][j]->w = widthEachSprite;
			rectArrays[i][j]->h = heightEachSprite;
		}
	}

	int poopFartRectScaleDown = 2;
	dstRect->x = 160;
	dstRect->y = 198;
	dstRect->w = widthEachSprite / poopFartRectScaleDown + 8;
	dstRect->h = heightEachSprite / poopFartRectScaleDown + 20;
}

unsigned int PoopFart::getToReleaseState() {
	return toRelease->state;
}

void PoopFart::render(SDL_Renderer* renderer) {

	if (finishRendering) {
		toRelease->state = getRandomNumber();
		finishRendering = false;
	}

	SDL_Rect* currentClip = rectArrays[toRelease->state][frameCount / EACH_FRAME_DURATION];

	SDL_RenderCopy(renderer, mTexture, currentClip, dstRect);

	++frameCount;

	if (frameCount / EACH_FRAME_DURATION >= FRAMES_PER_ANIMATION) {
		finishRendering = true;
		frameCount = 0;
	}


}

void PoopFart::close() {
	for (int i = 0; i < ANIMATION_PARTS; i++) {
		for (int j = 0; j < FRAMES_PER_ANIMATION; j++) {
			delete rectArrays[i][j];
		}
	}

	delete toRelease;
	delete dstRect;
}