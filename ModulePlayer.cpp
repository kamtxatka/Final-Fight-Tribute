#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"


// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer(bool active) : Module(active)
{
	groundedIdleState = { 91, 3, 45, 86 };
	groundedWalkingState = { 176, 97, 42, 85 };
	airIdleState = { 155, 188, 32, 93 };
	kickIdleState = { 194, 2, 60, 89 };
	airKickIdleState = { 54, 277,  47, 76 };


	currentIdleState = &groundedIdleState;

	position = { 150, SCREEN_HEIGHT - STREET_DEPTH, 0 };
	depth = 10;
	bodyWidth = groundedIdleState.w;
	dead = false;
	fistOffsetX = 30;
	fistOffsetY = -55;
	fistWidth = 40;
	fistHeight = 25;

	horizontalInput = verticalInput = 0;
	jumpInput = isJumping = false;
	jumpForce = 75;
	attackInput = isAttacking = false;
	timeBetweenAttacks = 100;
	speed = 1;

	canGoFront = canGoBack = canGoRight = canGoLeft = canGoUp = canGoDown = true;

	flipped = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Sprites/Personajes/FFOne_Guy.gif");

	collider = App->collision->AddCollider({ position.x, position.y ,30, 14}, 
		position.z, depth, PLAYER_MASK, std::bind(&ModulePlayer::OnCollisionTrigger, this, std::placeholders::_1, std::placeholders::_2));

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

void ModulePlayer::OnCollisionTrigger(CollisionMask collisionMask, iPoint collidedFrom)
{
	if (collisionMask == OBSTACLE_MASK || collisionMask == ENEMY_MASK)
	{
		collidedFrom.BloquedFrom();
		if (collidedFrom.x < 0)
			canGoLeft = false;
		if (collidedFrom.x > 0)
			canGoRight = false;
		if (collidedFrom.y < 0)
			canGoUp = false;
		if (collidedFrom.y > 0)
			canGoDown = false;
		if (collidedFrom.z < 0)
			canGoFront = false;
		if (collidedFrom.z > 0)
			canGoBack = false;
	}

}

// Update: draw background
update_status ModulePlayer::Update()
{

	//InputManageForTesting();

	CheckInputs();

	Move();
	Jump();
	Attack();
	

	//we actualize the collider's location
	collider->rect.x = position.x;
	collider->rect.y = position.y;
	collider->z = position.z;


	canGoFront = canGoBack = canGoRight = canGoLeft = canGoUp = canGoDown = true;


	
	// Draw everything --------------------------------------
	if (dead == false)
	{
		if (!flipped)
			App->renderer->AddBlitCall(graphics, position, currentIdleState);
		else
			App->renderer->AddBlitCall(graphics, {position.x + bodyWidth - currentIdleState->w, position.y, position.z}, currentIdleState, 1.0f, flipped);

	}


	return UPDATE_CONTINUE;
}


void ModulePlayer::CheckInputs()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		horizontalInput -= speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		horizontalInput += speed;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		verticalInput -= speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		verticalInput += speed;

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		attackInput = true;
	else
		attackInput = false;

	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
		jumpInput = true;
	else
		jumpInput = false;
}


void ModulePlayer::Move()
{
	if (isAttacking)
	{
		return;
	}

	if ((horizontalInput < 0 && canGoLeft) || (horizontalInput > 0 && canGoRight))
		position.x += horizontalInput;

	if ((!flipped && horizontalInput < 0) || (flipped && horizontalInput >0))
		flipped = !flipped;


	if ((verticalInput < 0 && canGoFront) || (verticalInput > 0 && canGoBack))
	{
		if ( ((verticalInput == 1 && position.z <= STREET_DEPTH) || (verticalInput == -1 && position.z >= 0)) && (!isJumping) )
		{
			position.z += verticalInput;
		}
	}
	
	if (!isJumping)
	{
		if (horizontalInput == 0 && verticalInput == 0)
			currentIdleState = &groundedIdleState;
		else
			currentIdleState = &groundedWalkingState;
	}

	horizontalInput = 0;
	verticalInput = 0;


}

void ModulePlayer::Jump()
{
	if (jumpInput && !isJumping && !isAttacking && canGoUp)
	{
		isJumping = true;
		position.y -= jumpForce;
		canGoDown = true;
		currentIdleState = &airIdleState;
	}

	if (isJumping || (canGoDown && position.y < SCREEN_HEIGHT - STREET_DEPTH))
	{
		if (position.y < SCREEN_HEIGHT - STREET_DEPTH && canGoDown)
			position.y += 2;
		else
		{
			isJumping = false;
			if (isAttacking)
			{
				isAttacking = false;
			}
		}
	}

	jumpInput = false;

}

void ModulePlayer::Attack()
{
	if (attackInput && !isAttacking)
	{
		isAttacking = true;

		if (isJumping)
			currentIdleState = &airKickIdleState;
		else
		{
			attackTimer.Start();
			currentIdleState = &kickIdleState;

			SDL_Rect fistRect = { 0,0,0,0 };
			if (!flipped)
				fistRect = { position.x + fistOffsetX, position.y + fistOffsetY, fistWidth, fistHeight };
			else
				fistRect = { position.x + bodyWidth - fistOffsetX - fistWidth, position.y + fistOffsetY, fistWidth, fistHeight };

			fistCollider = App->collision->AddCollider(fistRect, position.z, depth, PLAYER_ATTACK_MASK,
				std::bind(&ModulePlayer::OnCollisionTrigger, this, std::placeholders::_1, std::placeholders::_2));

		}
	}

	if (isAttacking && !isJumping)
	{
		if (attackTimer.GetTime() > timeBetweenAttacks)
		{
			attackTimer.Stop();
			isAttacking = false;
			fistCollider->to_delete = true;
		}
	}

	attackInput = false;

}

bool ModulePlayer::Grounded()
{
	return false;
}







































void ModulePlayer::InputManageForTesting()
{
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		collider->rect.x = position.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
		collider->rect.x = position.x;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.z += speed;
		collider->z = position.z;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.z -= speed;
		collider->z = position.z;
	}


	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
	{
		position.y -= speed;
		collider->rect.y = position.y;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		position.y += speed;
		collider->rect.y = position.y;
	}


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
		iPoint location = { position.x + 40, position.y - this->currentIdleState->h + 20, position.z };
		App->particles->AddParticle(App->particles->laserShot, location);
	}
}