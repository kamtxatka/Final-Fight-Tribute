#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleEnemy.h"



ModuleEnemy::ModuleEnemy()
{}

ModuleEnemy::~ModuleEnemy()
{}


bool ModuleEnemy::Start()
{
	LOG("Loading enemies");
	graphics = App->textures->Load("Sprites/Personajes/FFOne_Guy.gif");

	guy.groundedIdle = { 91, 3, 45, 86 };
	guy.groundedWalking = { 176, 97, 42, 85 };
	guy.atackingIdle = { 194, 2, 60, 89 };
	guy.currentIdle = &(guy.groundedIdle);
	guy.position = { 150, SCREEN_HEIGHT - STREET_DEPTH, 0 };
	guy.depth = 10;
	guy.bodyWidth = guy.groundedIdle.w;
	guy.bodyHeight = guy.groundedIdle.h;
	guy.fistOffsetX = 30;
	guy.fistOffsetY = -55;
	guy.fistWidth = 40;
	guy.fistHeight = 25;
	guy.timeBetweenAttacks = 500;
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

	ob->collider = App->collision->AddCollider({location.x,location.y, ob->bodyWidth, ob->bodyHeight}, ob->position.z, ob->depth, ENEMY_MASK,
		std::bind(&Enemy::OnCollisionTrigger, ob, std::placeholders::_1, std::placeholders::_2));
	ob->collider->SetPos(location.x, location.y, location.z);

	ob->hpSystem.Start();

	ob->decisionTimer.Start();

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
	this->bodyHeight = e.bodyHeight;
	this->canGoBack = canGoFront = canGoLeft = canGoRight = true;
	this->collider = nullptr;
	this->currentIdle = e.currentIdle;
	this->current_animation = e.current_animation;
	this->dead = false;
	this->depth = e.depth;
	this->dmged = false;

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

	if (decisionTimer.GetTime() > 200)
	{
		TakeDecision();
		decisionTimer.Stop();
		decisionTimer.Start();
	}
	Move();
	Attack();

	canGoFront = canGoBack = canGoRight = canGoLeft = true;
	dmged = (!(getHitTimer.GetTime() > 500 || getHitTimer.GetTime() == 0));


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
	/*if (otherCollisionMask == PLAYER_ATTACK_MASK)
	{
		this->collider->to_delete = true;
		this->fistCollider->to_delete = true;
		this->to_delete = true;
	}*/

	if (otherCollisionMask == OBSTACLE_MASK || otherCollisionMask == PLAYER_MASK)
	{
		collidedFrom.BloquedFrom();
		if (collidedFrom.x > 0)
			canGoLeft = false;
		if (collidedFrom.x < 0)
			canGoRight = false;
		if (collidedFrom.z > 0)
			canGoFront = false;
		if (collidedFrom.z < 0)
			canGoBack = false;
	}

	if (otherCollisionMask == PLAYER_ATTACK_MASK)
	{
		if (!dmged)
		{
			bool isDead = hpSystem.TakeDamage(10);
			if (isDead)
			{
				if (collider != nullptr)
					this->collider->to_delete = true;
				if (fistCollider != nullptr)
					this->fistCollider->to_delete = true;

				this->to_delete = true;
			}
			getHitTimer.Start();
			dmged = true;
		}



		//this->Disable();

	}


}

void Enemy::TakeDecision()
{

	horizontalInput = 0;
	verticalInput = 0;
	attackInput = false;

	if (App->player->position.DistanceTo(position) > bodyWidth + 10 && App->player->position.DistanceTo(position) < 250)
	{
		if (App->player->position.x > position.x)
			horizontalInput += speed;
		else if (App->player->position.x < position.x)
			horizontalInput -= speed;
		else
			horizontalInput = 0;

		if (App->player->position.z > position.z)
			verticalInput += speed;
		else if (App->player->position.z < position.z)
			verticalInput -= speed;
		else
			verticalInput = 0;
	}
	else
		attackInput = true;
}

void Enemy::Move()
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
		if (((verticalInput == 1 && position.z <= STREET_DEPTH) || (verticalInput == -1 && position.z >= 0)))
		{
			position.z += verticalInput;
		}
	}


	if (horizontalInput == 0 && verticalInput == 0)
		currentIdle = &groundedIdle;
	else
		currentIdle = &groundedWalking;


	/*horizontalInput = 0;
	verticalInput = 0;*/
}

void Enemy::Attack()
{
	LOG("attacattion");

	if (((position.x < App->player->position.x) && (flipped)) || ((position.x > App->player->position.x) && (!flipped)))
	{
		flipped = !flipped;
	}

	if (attackInput && !isAttacking)
	{
		isAttacking = true;

		attackTimer.Start();
		currentIdle = &atackingIdle;

		SDL_Rect fistRect = { 0,0,0,0 };
		if (!flipped)
			fistRect = { position.x + fistOffsetX, position.y + fistOffsetY, fistWidth, fistHeight };
		else
			fistRect = { position.x + bodyWidth - fistOffsetX - fistWidth, position.y + fistOffsetY, fistWidth, fistHeight };

		fistCollider = App->collision->AddCollider(fistRect, position.z, depth, ENEMY_ATTACK_MASK,
			std::bind(&Enemy::OnCollisionTrigger, this, std::placeholders::_1, std::placeholders::_2));

	}

	if (isAttacking)
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
