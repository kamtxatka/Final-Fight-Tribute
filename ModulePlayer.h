#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "Module.h"
#include "Animation.h"
#include "ModuleCollision.h"
#include "Point.h"

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

	void CheckInputs();
	void Move();
	void Jump();
	void Attack();
	bool Grounded();
	

public:

	SDL_Texture* graphics = nullptr;
	Animation* current_animation = nullptr;
	SDL_Rect* currentIdleState = nullptr;
	SDL_Rect groundedIdleState;
	SDL_Rect airIdleState;
	SDL_Rect kickIleState;

	CharacterState currentState = IDLE;
	CharacterState oldState = IDLE;
	iPoint position = { 0,0,0 };
	int depth = 0;
	bool dead = false;
	Collider* collider = nullptr;
	int speed = 0;

private:
	float horizontalInput = 0;
	float verticalInput = 0;
	bool jumpInput = false;
	bool attackInput = false;
};

#endif