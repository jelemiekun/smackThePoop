#pragma once
struct InputFlags {
	unsigned int playing : 1;

	unsigned int poopInProgress : 1;
	unsigned int poopFinished : 1;
	unsigned int FXInProgress : 1;
	unsigned int FXFinished : 1;
	unsigned int isFXPoopFinished : 1;
	unsigned int kindOfPoop : 2; // 0 - Brown, 1 - Green, 2 - Red, 3 - Brown
	unsigned int takeDamage : 1;
	unsigned int takeDamageInProgress : 1;
	unsigned int takeDamageFinished : 1;
	unsigned int animateSlap : 1;
	unsigned int animateSlapInProgress : 1;
	unsigned int animateSlapFinished : 1;
};