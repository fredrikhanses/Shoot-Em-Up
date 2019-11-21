#pragma once
#include "Texture.h"
#include <map>
#include <vector>
#include <iostream>


class Collider : public GameEntity
{
public:

	enum TAG
	{
		player,
		enemy,
		playerProjectile,
		enemyProjectile
	};

private:

	static Collider* sInstance;

	Texture* mDebugBox = nullptr;
	std::vector<Texture*> mDebugBoxes;

	std::map<Texture*, TAG> mColliders;

public:

	static Collider* Instance();
	static void Release();

	void AddCollider(Texture* tex, TAG tag);
	bool CollisionCheck(Texture* tex, TAG tag);

	void Update();
	void Render();

private:

	Collider();
	~Collider();

	// debug purpose
	std::string tagEnumToStr(int index);

};