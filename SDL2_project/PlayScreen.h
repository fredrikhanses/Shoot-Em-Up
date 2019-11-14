#pragma once
#include "Timer.h"
#include "ScoreBoard.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "BackgroundScroll.h"

class PlayScreen : public GameEntity
{
private:

	Timer* mTimer = nullptr;
	InputManager* mInputManager = nullptr;
	AudioManager* mAudioManager = nullptr;
	ScoreBoard* mScoreBoard = nullptr;
	Texture* mStartLabel = nullptr;
	//Texture* mBottomBarBackground = nullptr;

	// Blinker
	float mBlinkTimer;
	float mBlinkInterval;
	bool m1UPVisible;

	// Life
	static const int MAX_LIFE_TEXTURES = 3;
	GameEntity* mLives = nullptr;
	Texture* mLifeTextures[MAX_LIFE_TEXTURES];
	int mTotalLives;

	// level
	float mLevelStartTimer;
	float mLevelStartDelay;
	bool mGameStarted;
	bool mLevelStarted;
	int mCurrentStage;

private:

	void StartNextLevel();

public:

	PlayScreen();
	~PlayScreen();

	void StartNewGame();

	void SetHighScore(int score);
	void SetPlayerScore(int score);
	void SetLives(int lives);

	void Update();

	void Render();
};

