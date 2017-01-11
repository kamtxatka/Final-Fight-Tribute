#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include <algorithm>

using namespace std;

ModuleCollision::ModuleCollision()
{
	collisionMatrix.insert(make_pair(PLAYER_MASK, ENEMY_MASK));
	collisionMatrix.insert(make_pair(PLAYER_MASK, ENEMY_MASK));
	collisionMatrix.insert(make_pair(PLAYER_MASK, ENEMY_ATTACK_MASK));
	collisionMatrix.insert(make_pair(PLAYER_MASK, OBSTACLE_MASK));
	collisionMatrix.insert(make_pair(PLAYER_MASK, WALL_MASK));

	collisionMatrix.insert(make_pair(PLAYER_ATTACK_MASK, ENEMY_MASK));
	collisionMatrix.insert(make_pair(PLAYER_ATTACK_MASK, OBSTACLE_MASK));

	collisionMatrix.insert(make_pair(ENEMY_MASK, OBSTACLE_MASK));
	collisionMatrix.insert(make_pair(ENEMY_MASK, WALL_MASK));

	collisionMatrix.insert(make_pair(ENEMY_ATTACK_MASK, OBSTACLE_MASK));
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end();)
	{
		if ((*it)->to_delete == true)
		{
			RELEASE(*it);
			it = colliders.erase(it);
		}
		else
			++it;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	// TODO 8: Check collisions between all colliders. 
	// After making it work, review that you are doing the minumum checks possible

	for (std::list<Collider*>::iterator it1 = colliders.begin(); it1 != colliders.end(); ++it1)  
	{
		for (std::list<Collider*>::iterator it2 = std::next(it1); it2 != colliders.end(); ++it2)
		{

			//first we check if both masks can collide
			if (CheckCollisionMasks((*it1)->collisionMask,(*it2)->collisionMask))
			{
				//if so, do they collide?
				if ((*it1)->CheckCollision(*it2))
				{
					/*(*it1)->OnCollisionTrigger(**it2);
					(*it2)->OnCollisionTrigger(**it1);*/
				}
			}
		}
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()
{
	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
	{
		switch ((*it)->collisionMask)
		{
		case PLAYER_MASK:
			App->renderer->DrawQuad((*it)->rect, (*it)->z, (*it)->depth, 255, 250, 250, 50);
			break;
			/*case PLAYER_ATTACK_MASK:
			App->renderer->DrawQuad((*it)->rect, 255, 250, 250, 200);
			break;
			case ENEMY_MASK:
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 150);
			break;
			case ENEMY_ATTACK_MASK:
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 200);
			break;*/
		case OBSTACLE_MASK:
			App->renderer->DrawQuad((*it)->rect, (*it)->z, (*it)->depth, 0, 250, 0, 50);
			break;
			/*case WALL_MASK:
			App->renderer->DrawQuad((*it)->rect, 0, 250, 0, 200);
			break;
			default:
			App->renderer->DrawQuad((*it)->rect, 255, 250, 250, 150);
			break;*/
		}

	}
}

bool ModuleCollision::CheckCollisionMasks(const CollisionMask mask, const CollisionMask otherMask) const
{
	//Return true if both masks can collide
	bool ret = false;

	if (mask == PLAYER_ATTACK_MASK || otherMask == PLAYER_ATTACK_MASK)
	{
		ret = false;
	}

	std::set<std::pair<CollisionMask, CollisionMask>>::const_iterator it = collisionMatrix.find(make_pair(mask, otherMask));
	std::set<std::pair<CollisionMask, CollisionMask>>::const_iterator it2 = collisionMatrix.find(make_pair(otherMask, mask));

	//si uno de los dos iteradores ha encontrado algo es que pueden colisionar
	if (it != collisionMatrix.end() || it2 != collisionMatrix.end() )
	{
		ret = true;
	}

	return ret;
}


// Called before quitting
bool ModuleCollision::CleanUp()
{
	LOG("Freeing all colliders");

	for (list<Collider*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		RELEASE(*it);

	colliders.clear();

	return true;
}

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, const int& z, const int& depth,
	const CollisionMask collisionMask, std::function<void(CollisionMask, iPoint)> OnCollisionCallback)
{
	Collider* ret = new Collider(rect, z, depth, OnCollisionCallback);
	ret->collisionMask = collisionMask;

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const Collider* other) const
{
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
	// Return true if the argument and the own rectangle are intersecting

	bool ret = false;

	int LWall = this->rect.x;
	int RWall = this->rect.x + this->rect.w;
	int TWall = this->rect.y - this->rect.h;
	int DWall = this->rect.y;
	int FPoint = this->z;
	int BPoint = this->z - this->depth;

	int otherLWall = other->rect.x;
	int otherRWall = other->rect.x + other->rect.w;
	int otherTWall = other->rect.y - other->rect.h;
	int otherDWall = other->rect.y;
	int otherFPoint = other->z;
	int otherBPoint = other->z - other->depth;


	iPoint solapation = { 0,0,0 };
	bool colisionOnX = false;
	bool colisionOnY = false;
	bool colisionOnZ = false;


	if (!((BPoint >= otherFPoint) || (FPoint <= otherBPoint)))
		colisionOnZ = true;

	if (colisionOnZ)
	{
		if (!((LWall >= otherRWall) || (RWall <= otherLWall)))
			colisionOnX = true;
	}

	if (colisionOnZ && colisionOnX)
	{
		if (!((TWall >= otherDWall) || (DWall <= otherTWall)))
		{
			colisionOnY = true;
			ret = true;
		}
	}


	if (colisionOnZ)
		solapation.z = (FPoint > otherFPoint) ? BPoint - otherFPoint : FPoint - otherBPoint;
	if (colisionOnX)
		solapation.x = (RWall > otherRWall) ? LWall - otherRWall : RWall - otherLWall;
	if (colisionOnY)
		solapation.y = (DWall > otherDWall) ? TWall - otherDWall : DWall - otherTWall;


	//if (ret && this->collisionMask == PLAYER_MASK )
	//	LOG("solapation is x: %d,  y: %d,  z: %d  ", solapation.x, solapation.y, solapation.z);

	if (ret)
	{
		App->renderer->DrawQuad(rect, z, 0, 255, 250, 250, 255);
		App->renderer->DrawQuad(other->rect, other->z, other->depth, 255, 250, 250, 255);

		if (this->OnCollisionCallback != nullptr)
			this->OnCollisionCallback(other->collisionMask, solapation);
		if (other->OnCollisionCallback != nullptr)
			other->OnCollisionCallback(other->collisionMask, solapation);
	}


	//}
	return ret;
}


void Collider::OnCollisionTrigger(const Collider & otherCollider, iPoint solapation) const
{
	//App->renderer->DrawQuad(rect, z, 0, 255, 250, 250, 255);
	//App->renderer->DrawQuad(otherCollider.rect, 255, 250, 250, 255);


	//iPoint collidedFrom = { otherCollider.rect.x - this->rect.x, otherCollider.rect.y - this->rect.y, otherCollider.z - this->z };

	//if (this->OnCollisionCallback != nullptr)
		//this->OnCollisionCallback(otherCollider.collisionMask, collidedFrom);
	/*if (otherCollider.OnCollisionCallback != nullptr)
	otherCollider.OnCollisionCallback();*/
}
