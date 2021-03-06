#include "PlayScreen.h"

PlayScreen::PlayScreen()
{
	mTimer = Timer::Instance();
	mInputManager = InputManager::Instance();
	mAudioManager = AudioManager::Instance();
	mScoreBoard = ScoreBoard::Instance();
	mStartLabel = new Texture("START!", Graphics::Instance()->FONT_Emulogic, 64, { 255, 20, 147 });
	mStartLabel->Pos(Vector2D(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	// Blinker varialbles initializing
	mBlinkTimer = 0.0f;
	mBlinkInterval = 0.5f;
	m1UPVisible = true;

	for (int i = 0; i < MAX_LIFE_TEXTURES; i++)
	{
		// Player 1
		mLifeTextures[i] = new Texture("HeartOrange.png");
		mLifeTextures[i]->Parent(mScoreBoard->mPlayerOne);
		mLifeTextures[i]->Scale(VECTOR2D_ONE * 0.5f);
		mLifeTextures[i]->Pos(Vector2D(60.0f * (i % 3) + 120.0f, 70.0f * (i / 3)));

		// Player 2
		mLifeTextures2[i] = new Texture("HeartBlue.png");
		mLifeTextures2[i]->Parent(mScoreBoard->mPlayerTwo);
		mLifeTextures2[i]->Scale(VECTOR2D_ONE * 0.5f);
		mLifeTextures2[i]->Pos(Vector2D(-60.0f * (i % 3) - 120.0f, 70.0f * (i / 3)));
	}
	mPlayerLives = 3;
	mPlayer2Lives = 3;

	// level
	mLevel = nullptr;
	mLevelStartTimer = 0.0f;
	mLevelStartDelay = 1.0f;
	mGameStartTimer = 0.0f;
	mGameStarted = true;
	mLevelStarted = false;
	mCurrentStage = 0;

	// Player
	mPlayer = nullptr;
	mPlayer2 = nullptr;

	// background
	mBackgroundScroll = new BackgroundScroll();
}

PlayScreen::~PlayScreen()
{
	mTimer = nullptr;
	mAudioManager = nullptr;
	mScoreBoard = nullptr;

	delete mStartLabel;
	mStartLabel = nullptr;

	delete mLevel;
	mLevel = nullptr;

	delete mPlayer;
	mPlayer = nullptr;

	delete mPlayer2;
	mPlayer2 = nullptr;

	for (int i = 0; i < MAX_LIFE_TEXTURES; i++)
	{
		delete mLifeTextures[i];
		mLifeTextures[i] = nullptr;
		delete mLifeTextures2[i];
		mLifeTextures2[i] = nullptr;
	}

	// Background
	delete mBackgroundScroll;
	mBackgroundScroll = nullptr;
}

void PlayScreen::SetHighScore(unsigned int score)
{
	mScoreBoard->mHighScoreBoard->Score(score);
}

unsigned int PlayScreen::GetCurrentHighScore()
{
	return mScoreBoard->mHighScoreBoard->GetCurrentHighScore();
}

void PlayScreen::SetPlayerScore(unsigned int scorePlayer, unsigned int scorePlayer2)
{
	mScoreBoard->mPlayerOneScoreBoard->Score(scorePlayer);
	mScoreBoard->mPlayerTwoScoreBoard->Score(scorePlayer2);
}

void PlayScreen::SetLives(int playerlives, int player2lives)
{
	mPlayerLives = playerlives;
	mPlayer2Lives = player2lives;

	// Singleplayer
	if (mPlayer2 == nullptr)
	{
		// Player dies
		if (mPlayerLives <= 0)
		{
			mLevel->GameOver();
		}
	}

	// Multiplayer
	if (mPlayer2 != nullptr)
	{
		// Player 2 dies
		if (mPlayer2Lives <= 0)
		{
			mPlayer2->Active(false);
		}

		// Player dies
		if (mPlayerLives <= 0)
		{
			mPlayer->Active(false);
		}

		// Player and Player 2 dies
		if (mPlayerLives <= 0 && mPlayer2Lives <= 0)
		{
			mLevel->GameOver();
		}
	}
}

int PlayScreen::GetCurrentStageNum()
{
	return mCurrentStage;
}

void PlayScreen::StartNextLevel()
{
	// test 3 - increase stage level and create new level instance
	mCurrentStage++;
	if (mCurrentStage > 9)
	{
		mCurrentStage = 1;
	}
	mLevelStartTimer = 0.0f;
	mLevelStarted = true;

	// background change
	switch (mCurrentStage)
	{
	case 1:
		mPlayer->ToggleTexture(); // Change to mPlayerShip texture
		if (mPlayer2 != nullptr)
		{
			mPlayer2->ToggleTexture(); // Change to mPlayerShip2 texture
		}
		mBackgroundScroll->SetBackground("Backgrounds/space/Nebula_Red", 5);
		break;
	case 2:
		mBackgroundScroll->SetBackground("Backgrounds/mountain/mountain", 5);
		break;
	case 3:
		mPlayer->ToggleTexture(); // Change to mPlayer texture
		if (mPlayer2 != nullptr)
		{
			mPlayer2->ToggleTexture(); // Change to mPlayer2 texture
		}
		mBackgroundScroll->SetBackground("Backgrounds/demon/demon_woods", 4);
		break;
	case 4:
		mBackgroundScroll->SetBackground("Backgrounds/cyberpunk/cyberpunk", 3);
		break;
	case 5:
		mPlayer->ToggleTexture(); // Change to mPlayerShip texture
		if (mPlayer2 != nullptr)
		{
			mPlayer2->ToggleTexture(); // Change to mPlayerShip2 texture
		}
		mBackgroundScroll->SetBackground("Backgrounds/spaceblue/Nebula_Blue", 5);
		break;
	case 6:
		mBackgroundScroll->SetBackground("Backgrounds/spacepink/Nebula_Pink", 5);
		break;
	case 7:
		mBackgroundScroll->SetBackground("Backgrounds/planets/planets", 5);
		break;
	case 8:
		mPlayer->ToggleTexture(); // Change to mPlayer texture
		if (mPlayer2 != nullptr)
		{
			mPlayer2->ToggleTexture(); // Change to mPlayer2 texture
		}
		mBackgroundScroll->SetBackground("Backgrounds/yellowforest/yellowforest", 4);
		break;
	case 9:
		mBackgroundScroll->SetBackground("Backgrounds/forest/forest", 5);
		break;
	default:
		break;
	}

	// Create new Level
	delete mLevel;
	mLevel = new Level(mCurrentStage, mPlayer, mPlayer2);
}

void PlayScreen::StartNewGame(int mSelectMode)
{
	// Create new Players
	// Player 1
	delete mPlayer;
	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Pos(Vector2D(Graphics::Instance()->SCREEN_WIDTH * 0.08f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));
	mPlayer->Active(false);

	if (mSelectMode == 2)
	{
		// Player 2
		delete mPlayer2;
		mPlayer2 = new Player2();
		mPlayer2->Parent(this);
		mPlayer2->Pos(Vector2D(Graphics::Instance()->SCREEN_WIDTH * 0.08f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f + 100));
		mPlayer2->Active(false);
	}
	else
	{
		delete mPlayer2;
		mPlayer2 = nullptr;
	}

	// Initial setup
	SetLives(mPlayer->Lives(), (mSelectMode == 2) ? mPlayer2->Lives() : 3);
	SetPlayerScore(mPlayer->Score(), (mSelectMode == 2) ? mPlayer2->Score() : 0);

	BackgroundScroll::mScroll = false;

	mGameStarted = false;
	mLevelStarted = false;
	mLevelStartTimer = 0.0f;
	mCurrentStage = 0;

	mAudioManager->PlayMusic("Audios/playSceneStartDrum.wav", 0);
	mAudioManager->MusicVolume(10);
}

bool PlayScreen::GameOver()
{
	if (!mLevelStarted)
		return false;

	if (mLevel->State() == Level::gameover)
	{
		// Write highscore to file
		mScoreBoard->mHighScoreBoard->WriteHighScoreToFile();
		return true;
	}
	else
	{
		return false;
	}
}

bool PlayScreen::Victory()
{
	if (!mLevelStarted)
		return false;

	if (mLevel->State() == Level::victory)
	{
		// Write highscore to file
		mScoreBoard->mHighScoreBoard->WriteHighScoreToFile();
		return true;
	}
	else
	{
		return false;
	}
}

void PlayScreen::Update()
{
	// test 2 - wait mLevelStartDelay(1.0f) and start level
	if (mGameStarted)
	{
		if (!mLevelStarted)
		{
			mLevelStartTimer += mTimer->DeltaTime();
			if (mLevelStartTimer >= mLevelStartDelay)
			{
				StartNextLevel();
			}
		}
		else
		{
			mBackgroundScroll->Update();
			mLevel->Update();
			if (mLevel->State() == Level::finished)
			{
				mLevelStarted = false;
			}

			mPlayer->Update();
			if (mPlayer2 != nullptr)
				mPlayer2->Update();
		}

		// here do something between game starts and level starts(currently for 1 seconds = mLevelStartDelay)

	}
	else
	{
		mGameStartTimer += mTimer->DeltaTime();
		// if music is done || wait 2.5 sec to game starts
		if (!Mix_PlayingMusic() || mGameStartTimer > 2.5f)
		{
			mGameStarted = true;
			mGameStartTimer = 0.0f;
		}
	}

	// Blinker logic
	//mBlinkTimer += mTimer->DeltaTime();
	//if (mBlinkTimer >= mBlinkInterval)
	//{
	//	m1UPVisible = !m1UPVisible;
	//	mBlinkTimer = 0.0f;
	//}


	if (mPlayerLives > 0 || mPlayer2Lives > 0)
	{
		SetLives(mPlayer->Lives(), (mPlayer2 != nullptr) ? mPlayer2->Lives() : 0);
	}

	SetPlayerScore(mPlayer->Score(), (mPlayer2 != nullptr) ? mPlayer2->Score() : 0);

	// Update HighScore
	if (mPlayer->Score() > GetCurrentHighScore())
	{
		SetHighScore(mPlayer->Score());
	}

	if (mPlayer2 != nullptr && (mPlayer2->Score() > GetCurrentHighScore()))
	{
		SetHighScore(mPlayer2->Score());
	}

}

void PlayScreen::Render()
{
	// Blinker applying
	//if (m1UPVisible)
	//	mScoreBoard->Render();

	if (!mGameStarted)
	{
		mStartLabel->Render();
	}

	if (mGameStarted)
	{
		if (mLevelStarted)
		{
			mBackgroundScroll->Render();
			mLevel->Render();
			mScoreBoard->Render();

			// Player 1
			for (int i = 0; i < MAX_LIFE_TEXTURES && i < mPlayerLives; i++)
			{
				mLifeTextures[i]->Render();
			}
			mPlayer->Render();

			// Player 2
			if (mPlayer2 != nullptr)
			{
				for (int i = 0; i < MAX_LIFE_TEXTURES && i < mPlayer2Lives; i++)
				{
					{
						mLifeTextures2[i]->Render();
					}
				}
				mPlayer2->Render();
			}
		}
	}
}
