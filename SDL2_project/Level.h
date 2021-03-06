#pragma once
#include "ScoreBoard.h"
#include "BackgroundScroll.h"
#include "Player.h"
#include "Player2.h"
#include "Box.h"

class Level : public GameEntity
{
public:

	enum LEVEL_STATES
	{
		running, 
		finished, 
		gameover,
		victory
	};

private:

	Timer* mTimer = nullptr;
	AudioManager* mAudioManager = nullptr;

	LEVEL_STATES mCurrentState;

	int mCurrentStage = 1;
	bool mStageStarted;

	float mLabelTimer;

	// StageLabel
	Texture* mStageLabel = nullptr;
	ScoreManager* mStageNumber = nullptr;
	float mStageLabelOnScreen;
	float mStageLabelOffScreen;

	// ReadyLabel
	Texture* mReadyLabel = nullptr;
	float mReadyLabelOnScreen;
	float mReadyLabelOffScreen;

	// GoLabel
	Texture* mGoLabel = nullptr;
	float mGoLabelOnScreen;
	float mGoLabelOffScreen;

	// Player - currently using this just to make it active
	Player* mPlayer = nullptr;
	Player2* mPlayer2 = nullptr;

	// GameOverLabel
	Texture* mGameOverLabel = nullptr;
	bool mGameOver;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	// Victory
	Texture* mVictoryLabel = nullptr;
	bool mVictory;
	float mVictoryDelay;
	float mVictoryTimer;
	float mVictoryLabelOnScreen;

	// Enemy
	float mSpawnTime;
	float mSpawnTimer;
	float mCurrentEnemiesCount;
	float mMaxEnemies;
	std::vector<Box*> mBoxes;

private:

	void StartStage();
	void HandleStartLabels();

public:

	Level(int stage, Player* player, Player2* player2);
	~Level();

	LEVEL_STATES State();

	bool HasAllPlayersDied();
	void GameOver();
	void Victory();
	void LevelWon();

	void Update();
	void Render();

};

