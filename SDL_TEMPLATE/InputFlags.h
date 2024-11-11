#pragma once
struct InputFlags {
	unsigned int playing : 1;

	unsigned int inStart : 1;
	unsigned int inGameOver : 1;
	unsigned int inSettings : 1;

	unsigned int win : 1;

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

	unsigned int GOBGoutside : 1;
	unsigned int GOBGinside : 2;

	unsigned int SMoutside : 1;
	unsigned int SMinPlay : 1;
	unsigned int SMinSett : 1;
	unsigned int SMinQuit : 1;
	unsigned int SMinPlayClick : 1;
	unsigned int SMinSettClick : 1;
	unsigned int SMinQuitClick : 1;

	unsigned int readyToChangeMusic : 1;
};