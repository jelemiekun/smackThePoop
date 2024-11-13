#include "PoopBar.h"
#include "App_Info.h"
#include <iostream>

constexpr uint8_t BORDER_MARGIN = 5;

PoopBar::PoopBar() : mTexture(nullptr), rectBorder(nullptr), poopRemaining(POOP_COUNT) {
	for (int i = 0; i < POOP_COUNT; i++) {
		rect[i] = nullptr;
	}
}

PoopBar::~PoopBar() {}

void PoopBar::updateEachRectPoop() {
	for (int i = 0; i < POOP_COUNT; i++) {
		rect[i] = new SDL_Rect;

		rect[i]->x = (((rectBorder->w / POOP_COUNT) * i) + BORDER_MARGIN) + rectBorder->x;
		rect[i]->y = BORDER_MARGIN + rectBorder->y;
		rect[i]->w = (rectBorder->w / POOP_COUNT) - BORDER_MARGIN;
		rect[i]->h = rectBorder->h - (BORDER_MARGIN * 2);
	}
}

void PoopBar::init(SDL_Renderer*& renderer, const Uint16& SCREEN_WIDTH) {
	mTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
		(SCREEN_WIDTH / 30) * 16, 50);

	if (mTexture == nullptr) {
		std::cout << "Failed to create texture: " << SDL_GetError() << '\n';
		return;
	}

	rectBorder = new SDL_Rect;
	rectBorder->x = 0;
	rectBorder->y = 0;

	SDL_QueryTexture(mTexture, NULL, NULL, &rectBorder->w, &rectBorder->h);
}

void PoopBar::setXY(const int &x, const int &y) {
	rectBorder->x = x;
	rectBorder->y = y;
	updateEachRectPoop();
}

void PoopBar::render(SDL_Renderer*& renderer) {
	SDL_SetRenderTarget(renderer, mTexture);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, rectBorder);

	SDL_SetRenderDrawColor(renderer, 136, 69, 19, 255);

	for (int i = 0; i < POOP_COUNT; i++) {
		SDL_RenderDrawRect(renderer, rect[i]);
	}

	SDL_SetRenderDrawColor(renderer, 168, 82, 2, 255);
	for (int i = 0; i < poopRemaining; i++) {
		SDL_RenderFillRect(renderer, rect[i]);
	}

	SDL_SetRenderTarget(renderer, nullptr);
}

void PoopBar::poopReleased() {
	--poopRemaining;
}

void PoopBar::resetPoopBar() {
	poopRemaining = POOP_COUNT;
}

uint8_t PoopBar::getPoopRemaining() const {
	return poopRemaining;
}

void PoopBar::close() {
	SDL_DestroyTexture(mTexture);
	delete rectBorder;
	for (int i = 0; i < POOP_COUNT; i++) {
		delete rect[i];
	}
}