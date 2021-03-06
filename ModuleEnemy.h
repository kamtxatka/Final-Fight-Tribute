#ifndef __MODULE_ENEMY__
#define __MODULE_ENEMY__

#include "Module.h"
#include "Animation.h"
#include "ModuleCollision.h"
#include "CustomCounter.h"
#include "HPSystem.h"

struct SDL_Texture;

struct Enemy
{
	bool to_delete = false;

	SDL_Rect* currentIdle = nullptr;
	SDL_Rect groundedIdle = { 0,0,0 };
	SDL_Rect groundedWalking = { 0,0,0 };
	SDL_Rect atackingIdle = { 0,0,0 };

	Animation* current_animation = nullptr;
	Animation walking_animation;
	Animation attacking_animation;
	//there is no jump or air kick for enemies... yet
	CustomCounter attackTimer = CustomCounter();
	CustomCounter decisionTimer = CustomCounter();
	CustomCounter getHitTimer = CustomCounter();

	iPoint position = { 0,0,0 };
	int depth = 0;
	int bodyWidth, bodyHeight = 0;
	bool dead = false;
	HPSystem hpSystem = HPSystem(300);

	Collider* collider = nullptr;
	Collider* fistCollider = nullptr;
	int fistOffsetX, fistOffsetY, fistWidth, fistHeight = 0;

	int horizontalInput, verticalInput = 0;
	bool attackInput, isAttacking = false;
	Uint32 timeBetweenAttacks = 0;
	float speed = 0;

	bool canGoFront, canGoBack, canGoRight, canGoLeft = false;

	bool flipped = false;
	bool dmged = false;


	Enemy();
	Enemy(const Enemy& e);
	~Enemy();
	bool Update();
	void OnCollisionTrigger(CollisionMask otherCollisionMask, iPoint collidedFrom);

	void TakeDecision();
	void Move();
	void Attack();

};



class ModuleEnemy :
	public Module
{
public:
	ModuleEnemy();
	~ModuleEnemy();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddEnemy(const Enemy& enemy, iPoint location); // feel free to expand this call


private:
	SDL_Texture* graphics = nullptr;
	std::list<Enemy*> active;

public:
	Enemy guy;


};


#endif
