#include "PoopFart.h"
#include <random>

PoopFart::PoopFart() : toRelease(nullptr), frameCount(0), finishRendering(true),
					widthEachSprite(0), heightEachSprite(0), dstRect(nullptr) {
	for (int i = 0; i < POOPFART_ANIMATION_PARTS; i++) {
		for (int j = 0; j < POOPFART_FRAMES_PER_ANIMATION; j++) {
			rectArrays[i][j] = nullptr;
		}
	}
}

PoopFart::~PoopFart() {}

void PoopFart::generateRandomNumber() {
	static std::random_device dev;
	static std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, POOPFART_ANIMATION_PARTS - 1);

	int num = dist6(rng);

	if (num == 3) num += 2;
	else if (num == 4) num++;
	toRelease->state = num;
}

void PoopFart::init() {
	toRelease = new ToRelease;

	for (int i = 0; i < POOPFART_ANIMATION_PARTS; i++) {
		for (int j = 0; j < POOPFART_FRAMES_PER_ANIMATION; j++) {
			rectArrays[i][j] = new SDL_Rect;
		}
	}

	dstRect = new SDL_Rect;
}

void PoopFart::initSpriteSheet() {
	widthEachSprite = srcRect->w / POOPFART_FRAMES_PER_ANIMATION;
	heightEachSprite = srcRect->h / POOPFART_ANIMATION_PARTS;

	for (int i = 0; i < POOPFART_ANIMATION_PARTS; i++) {
		for (int j = 0; j < POOPFART_FRAMES_PER_ANIMATION; j++) {
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

bool PoopFart::isFinishRendering() const {
	return finishRendering;
}

void PoopFart::setFinishRendering(const bool &finish) {
	finishRendering = finish;
}

void PoopFart::setReleaseState(const int &state) {
	toRelease->state = state;
}

void PoopFart::render(SDL_Renderer*& renderer) {
	if (finishRendering) {
		finishRendering = false;
	}

	SDL_Rect currentClip = *rectArrays[toRelease->state][frameCount / POOPFART_EACH_FRAME_DURATION];


	++frameCount;

	if (frameCount / POOPFART_EACH_FRAME_DURATION >= POOPFART_FRAMES_PER_ANIMATION) {
		finishRendering = true;
		frameCount = 0;
	} else {
		// Memory Leak Here
		 SDL_RenderCopy(renderer, *&mTexture, &currentClip, dstRect);
	}
}

void PoopFart::close() {
	for (int i = 0; i < POOPFART_ANIMATION_PARTS; i++) {
		for (int j = 0; j < POOPFART_FRAMES_PER_ANIMATION; j++) {
			delete rectArrays[i][j];
		}
	}

	delete toRelease;
	delete dstRect;
}