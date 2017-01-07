#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleScene1_1.h"


ModuleScene1_1::ModuleScene1_1(bool active): Module(active)
{
	background.x = 64;
	background.y = 0;
	background.w = 1296;
	background.h = 257;

	sky.x = 3;
	sky.y = 0;
	sky.w = 256;
	sky.h = 150;

}

ModuleScene1_1::~ModuleScene1_1()
{}

bool ModuleScene1_1::Start()
{
	LOG("Loading scene1");

	backgroundgraphics = App->textures->Load("Sprites/Niveles/Slum1-1.png");
	skygraphics = App->textures->Load("Sprites/Niveles/FF_Stage1.png");

	App->player->Enable();
	App->particles->Enable();
	App->collision->Enable();

	App->audio->PlayMusic("Audio/GoAhead.ogg", 1.0f);

	return true;
}

bool ModuleScene1_1::CleanUp()
{
	LOG("Unloading scene1");

	App->textures->Unload(backgroundgraphics);
	App->textures->Unload(skygraphics); 

	App->player->Disable();
	App->collision->Disable();
	App->particles->Disable();

	return true;
}


update_status ModuleScene1_1::Update()
{

	/*App->renderer->Blit(skygraphics, 0, 0, &sky, 4.0f);
	App->renderer->Blit(skygraphics, 650, 0, &sky, 4.0f);
	App->renderer->Blit(skygraphics, 1050, 0, &sky, 4.0f);*/

	App->renderer->BlitStretch(skygraphics, new SDL_Rect{ 0, -20,256,200 }, &sky, 0.75f);
	App->renderer->BlitStretch(skygraphics, new SDL_Rect{ 450, -20,256,200 }, &sky, 0.75f);
	App->renderer->BlitStretch(skygraphics, new SDL_Rect{ 780, -20,256,200 }, &sky, 0.75f);

	App->renderer->Blit(backgroundgraphics, 0, -15, &background);


	return UPDATE_CONTINUE;
}


