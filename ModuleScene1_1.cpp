#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleObstacle.h"
#include "ModuleScene1_1.h"


ModuleScene1_1::ModuleScene1_1(bool active): Module(active)
{
	background = { 94, 0, 1296, 257 };

	sky = { 3, 0, 256, 150 };
	skyBlit1 = { 0, -20,256,200 };
	skyBlit2 = { 450, -20,256,200 };
	skyBlit3 = { 780, -20,256,200 };

	//barrel = { 30, 178, 32, 62 };


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
	App->obstacles->AddObstacle(App->obstacles->barrrel, { 100, SCREEN_HEIGHT / 2, 0 });
	App->obstacles->AddObstacle(App->obstacles->barrrel, { 50, SCREEN_HEIGHT / 2, 0 });


	//barrel = &(App->obstacles->barrel);

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
	//App->renderer->BlitStretch(skygraphics, &skyBlit1, &sky, 0.75f);
	App->renderer->BlitStretch(skygraphics, &skyBlit2, &sky, 0.75f);
	App->renderer->BlitStretch(skygraphics, &skyBlit3, &sky, 0.75f);

	//App->renderer->Blit(backgroundgraphics, 0, -15, &background);

	//App->renderer->Blit(App->obstacles->graphics, 100, 100, barrel);


	return UPDATE_CONTINUE;
}


