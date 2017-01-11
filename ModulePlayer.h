#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleCollision.h"
#include "Point.h"
#include "CustomCounter.h"


struct SDL_Texture;

enum CharacterState
{
	IDLE,
	MOVING,
	JUMPING,
	ATTACKING
};

class ModulePlayer : public Module
{
public:
	ModulePlayer(bool active = true);
	~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollisionTrigger(CollisionMask collisionMask, iPoint collidedFrom);

	void CheckInputs();
	void Move();
	void Jump();
	void Attack();
	bool Grounded();


	void InputManageForTesting();


	

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	SDL_Rect* currentIdleState = nullptr;
	SDL_Rect groundedIdleState = { 0,0,0 };
	SDL_Rect groundedWalkingState = { 0,0,0 };
	SDL_Rect airIdleState = { 0,0,0 };
	SDL_Rect kickIdleState = { 0,0,0 };
	SDL_Rect airKickIdleState = { 0,0,0 };
	CustomCounter attackTimer = CustomCounter();

	iPoint position = { 0,0,0 };
	int depth = 0;
	bool dead = false;
	CharacterState currentState = IDLE;
	CharacterState oldState = IDLE;
	Collider* collider = nullptr;
	Collider* fistCollider = nullptr;
	Collider* kickCollider = nullptr;

private:
	int horizontalInput , verticalInput = 0;
	bool jumpInput, isJumping = false;
	int jumpForce = 0;
	bool attackInput, isAttacking = false;
	Uint32 timeBetweenAttacks = 0;
	int speed = 0;

	bool canGoFront, canGoBack, canGoRight, canGoLeft, canGoUp, canGoDown = false;



};

#endif