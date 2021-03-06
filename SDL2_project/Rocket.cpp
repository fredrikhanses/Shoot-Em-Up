#pragma warning( push )
#pragma warning (disable : 4244 )

#include "Rocket.h"

std::vector<std::vector<Vector2D>> Rocket::sPaths;

Rocket::Rocket(Collider::TAG tag)
{
	mTimer = Timer::Instance();
	mAudioManager = AudioManager::Instance();

	mSpeed = 700.0f;

	mRocket = new AnimatedTexture("Fireball_68x9.png", 0, 0, 68, 9, 10, 0.2f, AnimatedTexture::horizontal);
	mRocket->Parent(this);
	mRocket->Scale(VECTOR2D_ONE * 1.5f);
	mRocket->Pos(VECTOR2D_ZERO);

	mCurrentPath = 0;
	mCurrentWayPoint = 0;
	midPoint = 0.0f;

	Reload();

	//collider
	AddCollider(new BoxCollider(mRocket->ScaledDimensions(), tag));
	if (tag == Collider::player1Projectile || tag == Collider::player2Projectile)
	{
		mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::PlayerProjectiles);
	}
	else if (tag == Collider::enemyProjectile)
	{
		mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::EnemyProjectiles);
	}
}

Rocket::~Rocket()
{
	mTimer = nullptr;
	mAudioManager = nullptr;

	delete mRocket;
	mRocket = nullptr;
}

void Rocket::CreatePath(Vector2D pos, int pathNum)
{
	Pos(pos);
	Active(true);
	mAudioManager->PlaySFX("Audios/fireball.wav", 0, 2);
	mAudioManager->SFXVolume(2, 80);

	if (Graphics::Instance()->SCREEN_WIDTH - pos.x < 200)
	{
		midPoint = (float)Graphics::Instance()->SCREEN_WIDTH;
	}
	else
	{
		midPoint = (Graphics::Instance()->SCREEN_WIDTH + pos.x) * 0.5f;
	}

	float ctrlPoint = ((Graphics::Instance()->SCREEN_WIDTH + 100) - pos.x) * 0.25f - 50.0f;

	Vector2D start = Vector2D(pos.x + 30, pos.y);
	Vector2D end = Vector2D((float)Graphics::Instance()->SCREEN_WIDTH + 100.0f, pos.y);
	Vector2D startCtrl = Vector2D((start.x + midPoint) * 0.5f, start.y - ctrlPoint);
	Vector2D endCtrl = Vector2D((midPoint + end.x) * 0.5f, start.y + ctrlPoint);

	BezierPath* path = new BezierPath();

	// add a vector in sPaths if its size is smaller than mCurrentPath(bulletIndex)
	if (sPaths.size() <= pathNum)
	{
		for (size_t i = 0; i <= pathNum; i++)
		{
			sPaths.push_back(std::vector<Vector2D>());
		}
	}

	mCurrentPath = pathNum;

	if (mCurrentPath % 2 == 0)
	{
		path->AddCurve({ start, startCtrl, endCtrl, end }, 25);	// down and up
	}
	else
	{
		path->AddCurve({ start, Vector2D(startCtrl.x, start.y + ctrlPoint), Vector2D(endCtrl.x, start.y - ctrlPoint), end }, 25); // up and down
	}

	path->Sample(&sPaths[mCurrentPath]);

	delete path;
}

void Rocket::Reload()
{
	Active(false);
}

void Rocket::Hit(PhysicsEntity* other)
{
	mCurrentWayPoint = 0;
	sPaths[mCurrentPath].clear();
	Reload();
}

bool Rocket::IgnoreCollisions()
{
	return !Active();
}

void Rocket::Update()
{

	if (Active())
	{
		if (mCurrentWayPoint < sPaths[mCurrentPath].size())
		{
			mRocket->Update();
			Vector2D distance = sPaths[mCurrentPath][mCurrentWayPoint] - Pos();
			Translate(distance.Normalized() * mTimer->DeltaTime() * mSpeed, world);

			// make projectile look at the path
			Rotation(atan2(distance.y, distance.x) * RAD_TO_DEG);

			if ((sPaths[mCurrentPath][mCurrentWayPoint] - Pos()).MagnitudeSqr() < EPSILON)
				mCurrentWayPoint++;

			if (mCurrentWayPoint >= sPaths[mCurrentPath].size())
			{
				// path is completed
			}
		}
		else
		{
			// when finished moving path
			mCurrentWayPoint = 0;
			sPaths[mCurrentPath].clear();
			Reload();
		}
	}
}

void Rocket::Render()
{
	if (Active())
	{
		mRocket->Render();

		// Debug colliderbox
		//PhysicsEntity::Render();
	}
}
#pragma warning( pop )