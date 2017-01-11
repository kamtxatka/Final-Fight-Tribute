#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleObstacle.h"



ModuleObstacle::ModuleObstacle()
{}

ModuleObstacle::~ModuleObstacle()
{
}

bool ModuleObstacle::Start()
{
	LOG("Loading obstacles");
	graphics = App->textures->Load("Sprites/Itemst.gif");

	barrel.collider = nullptr;
	barrel.destructible = false;
	barrel.location = {0,0};
	barrel.depth = 10;
	barrel.rect = { 30, 178, 32, 62 };
	barrel.to_delete = false;
	barrel.collisionMask = OBSTACLE_MASK;

	return true;
}


bool ModuleObstacle::CleanUp()
{
	LOG("Unloading scene1");
	App->textures->Unload(graphics);

	for (list<Obstacle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();
	return true;
}




update_status ModuleObstacle::Update()
{
	for (list<Obstacle*>::iterator it = active.begin(); it != active.end();)
	{
		Obstacle* p = *it;

		if (p->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
		{
			//App->renderer->Blit(graphics, p->location, &(p->rect));
			App->renderer->AddBlitCall(graphics, p->location, &(p->rect));

			++it;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleObstacle::AddObstacle(const Obstacle & prefab, iPoint location)
{
	Obstacle* ob = new Obstacle(prefab);
	ob->location = location;

	ob->collider = App->collision->AddCollider(ob->rect, ob->location.z, ob->depth, ob->collisionMask,
		std::bind(&Obstacle::OnCollisionTrigger, ob, std::placeholders::_1, std::placeholders::_2));
	ob->collider->SetPos(location.x, location.y, location.z);

	active.push_back(ob);

}

Obstacle::Obstacle(){}

Obstacle::Obstacle(const Obstacle& ob)
{
	this->collisionMask = ob.collisionMask;
	this->destructible = ob.destructible;
	this->location = ob.location;
	this->depth = ob.depth;
	this->rect = ob.rect;
	this->to_delete = ob.to_delete;
	//falta collider
}

Obstacle::~Obstacle()
{
}

bool Obstacle::Update()
{
	bool ret = true;
	//not much here...
	return ret;
}

void Obstacle::OnCollisionTrigger(CollisionMask collisionMask, iPoint collidedFrom)
{
	//LOG("Obstacle colision");
	//App->collision->AddCollider({ SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - 30, 10, 10 }, ENEMY_MASK, nullptr);
	
	//this->collider->to_delete = true;
	//this->to_delete = true;
}




