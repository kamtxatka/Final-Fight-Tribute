#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include <list>
#include <set>
#include <functional>
#include "Module.h"

enum CollisionMask
{
	PLAYER_MASK,
	PLAYER_ATTACK_MASK,
	ENEMY_MASK,
	ENEMY_ATTACK_MASK,
	OBSTACLE_MASK,
	WALL_MASK,
	DEFAULT_MASK
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	int z = 0;
	int depth = 0;
	bool to_delete = false;
	CollisionMask  collisionMask = DEFAULT_MASK;
	std::function<void()> OnCollisionCallback = nullptr;
	//void(*OnCollisionCallback)() = nullptr;


	// TODO 10: Add a way to notify other classes that a collision happened
	void OnCollisionTrigger(const Collider& otherCollider) const;


	Collider(SDL_Rect rectangle, int z, int depth, std::function<void()> OnCollisionCallback) :
		rect(rectangle), z(z), depth(depth), OnCollisionCallback(OnCollisionCallback)
	{}

	void SetPos(int x, int y, int z = 0)
	{
		rect.x = x;
		rect.y = y;
		z = z;
	}

	bool CheckCollision(const SDL_Rect& r, const int& z, const int& depth) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate();
	update_status Update();

	bool CleanUp();

	Collider* AddCollider(const SDL_Rect& rect, const int& z, const int& depth,
		const CollisionMask collisionMask, std::function<void()> OnCollisionCallback);

	void DebugDraw();
	bool CheckCollisionMasks(const CollisionMask mask, const CollisionMask otherMask) const;

private:
	std::list<Collider*> colliders;
	std::set <std::pair<CollisionMask, CollisionMask>> collisionMatrix;
	bool debug = true;
};

#endif // __ModuleCollision_H__