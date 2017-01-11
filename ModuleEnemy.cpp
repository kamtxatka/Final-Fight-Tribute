#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleEnemy.h"



ModuleEnemy::ModuleEnemy()
{}

ModuleEnemy::~ModuleEnemy()
{}


bool ModuleEnemy::Start()
{
	graphics = App->textures->Load("Sprites/Personajes/FFOne_Guy.gif");

	guy.groundedIdle = { 91, 3, 45, 86 };
	guy.groundedWalking = { 176, 97, 42, 85 };
	guy.atackingIdle = { 194, 2, 60, 89 };
	guy.currentIdle = &(guy.groundedIdle);
	guy.position = { 150, SCREEN_HEIGHT - STREET_DEPTH, 0 };
	guy.depth = 10;
	guy.bodyWidth = guy.groundedIdle.w;
	guy.fistOffsetX = 30;
	guy.fistOffsetY = -55;
	guy.fistWidth = 40;
	guy.fistHeight = 25;
	guy.timeBetweenAttacks = 100;
	guy.speed = 1;

	guy.walking_animation.frames.push_back({ 0,0,0,0 });
	guy.attacking_animation.frames.push_back({ 0,0,0,0 });

	return true;
}

bool ModuleEnemy::CleanUp()
{
	LOG("Unloading scene1");
	App->textures->Unload(graphics);

	for (list<Enemy*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();
	return true;
}


update_status ModuleEnemy::Update()
{
	for (list<Enemy*>::iterator it = active.begin(); it != active.end();)
	{
		Enemy* e = *it;

		if (e->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
		{

			if ((*it)->dead == false)
			{
				if (!(*it)->flipped)
					App->renderer->AddBlitCall(graphics, (*it)->position, (*it)->currentIdle);
				else
					App->renderer->AddBlitCall(graphics, { (*it)->position.x + (*it)->bodyWidth - (*it)->currentIdle->w, 
					(*it)->position.y, (*it)->position.z }, (*it)->currentIdle, 1.0f, true);

			}

			++it;
		}
	}

	return UPDATE_CONTINUE;
}


void ModuleEnemy::AddEnemy(const Enemy & prefab, iPoint location)
{
	Enemy* ob = new Enemy(prefab);
	ob->position = location;

	ob->collider = App->collision->AddCollider({location.x,location.y, ob->bodyWidth,14}, ob->position.z, ob->depth, ENEMY_MASK,
		std::bind(&Enemy::OnCollisionTrigger, ob, std::placeholders::_1, std::placeholders::_2));
	ob->collider->SetPos(location.x, location.y, location.z);

	active.push_back(ob);

}

Enemy::Enemy()
{}

Enemy::Enemy(const Enemy & e)
{
	to_delete = false;

	this->atackingIdle = e.atackingIdle;
	this->attacking_animation = e.attacking_animation;
	this->attackInput = false;
	this->bodyWidth = e.bodyWidth;
	this->canGoBack = canGoDown = canGoFront = canGoLeft = canGoRight = canGoUp = true;
	this->collider = nullptr;
	this->currentIdle = e.currentIdle;
	this->current_animation = e.current_animation;
	this->dead = false;
	this->depth = e.depth;

	this->fistCollider = nullptr;
	this->fistHeight = e.fistHeight;
	this->fistOffsetX = e.fistOffsetX;
	this->fistOffsetY = e.fistOffsetY;
	this->fistWidth = e.fistWidth;

	this->flipped = true;
	this->groundedIdle = e.groundedIdle;
	this->groundedWalking = e.groundedWalking;

	this->horizontalInput = 0;
	this->isAttacking = false;
	this->position = { 0,0,0 };
	this->speed = e.speed;
	this->timeBetweenAttacks = e.timeBetweenAttacks;
	this->verticalInput = 0;
	this->walking_animation = e.walking_animation;
}

Enemy::~Enemy()
{
}

bool Enemy::Update()
{
	bool ret = true;


	// if hp <= 0  to delete
	if (to_delete)
		ret = false;

	if (collider != nullptr)
	{
		collider->rect.x = position.x;
		collider->rect.y = position.y;
		collider->z = position.z;
	}

	return ret;
}

void Enemy::OnCollisionTrigger(CollisionMask otherCollisionMask, iPoint collidedFrom)
{
	LOG("particle colision");
}
