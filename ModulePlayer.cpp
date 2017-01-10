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
	depth = 40;
	dead = false;

	horizontalInput = 0;
	verticalInput = 0;
	jumpInput = false;
	isJumping = false;
	jumpForce = 75;
	attackInput = false;
	isAttacking = false;
	timeBetweenAttacks = 500;
	speed = 1;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Sprites/Personajes/FFOne_Guy.gif");

	collider = App->collision->AddCollider({ position.x, position.y ,30, 14}, position.z, depth, PLAYER_MASK, nullptr);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	App->textures->Unload(graphics);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	
	//InputManageForTesting();

	CheckInputs();

	Move();
	Jump();
	//Attack();
	
	// Draw everything --------------------------------------
	if (dead == false)
		App->renderer->AddBlitCall(graphics, position, currentIdleState);


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

	if (horizontalInput != 0)
	{
		position.x += horizontalInput;
		collider->rect.x = position.x;
	}
	if (verticalInput != 0)
	{
		if ( ((verticalInput == 1 && position.z <= STREET_DEPTH) || (verticalInput == -1 && position.z >= 0)) && (!isJumping) )
		{
			position.z += verticalInput;
			collider->z = position.z;
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
	if (jumpInput && !isJumping && !isAttacking)
	{
		isJumping = true;
		position.y -= jumpForce;
		currentIdleState = &airIdleState;
	}

	if (isJumping)
	{
		if (position.y < SCREEN_HEIGHT - STREET_DEPTH)
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
	collider->rect.y = position.y;

	jumpInput = false;

}

void ModulePlayer::Attack()
{
	if (attackInput && !isAttacking)
	{
		isAttacking = true;
		//start timer timeBetweenAttacks

		if (isJumping)
			currentIdleState = &airKickIdleState;
		else
		{
	//		attackTimer.Start();
	//		currentIdleState = &kickIdleState;
	//	}
	//}

	//if (isAttacking && !isJumping)
	//{
	//	if (attackTimer.GetTime() > timeBetweenAttacks)
	//	{
	//		attackTimer.Stop();
	//		isAttacking = false;
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