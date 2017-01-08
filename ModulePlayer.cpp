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
	groundedIdleState = {91,3,45,86};
	
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("Sprites/Personajes/FFOne_Guy.gif");

	dead = false;
	position.x = 150;
	position.y = 120;
	speed = 1;
	float horizontalInput = 0;
	float verticalInput = 0;
	bool jumpInput = false;
	bool attackInput = false;
	currentIdleState = &groundedIdleState;

	collider = App->collision->AddCollider({ position.x, position.y ,30, 14}, PLAYER_MASK, nullptr);

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
	//if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//{
	//	position.y += speed;
	//	collider->rect.y = position.y;
	//	if(current_animation != &down)
	//	{
	//		down.Reset();
	//		current_animation = &down;
	//	}
	//}

	CheckInputs();
	
	Move();


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// TODO 6: Shoot a laser using the particle system
		App->particles->AddParticle(App->particles->laserShot, position.x+ 40, position.y);
	}


	// Draw everything --------------------------------------
	if(dead == false)
		App->renderer->Blit(graphics, position.x, position.y, currentIdleState);

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
	if (horizontalInput)
	{
		position.x += (int)horizontalInput;
		collider->rect.x = position.x;
		horizontalInput = 0;
	}
	if (verticalInput)
	{
		position.y += (int)verticalInput;
		collider->rect.y = position.y;
		verticalInput = 0;
	}
}

void ModulePlayer::Jump()
{
}

void ModulePlayer::Attack()
{
	//
	if (true)
	{

	}

}

bool ModulePlayer::Grounded()
{
	return false;
}
