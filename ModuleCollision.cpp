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
				if ((*it1)->CheckCollision((*it2)->rect))
				{
					(*it1)->OnCollisionTrigger(**it2);
					//(*it2)->OnCollisionTrigger(**it1);
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
			App->renderer->DrawQuad((*it)->rect, 255, 250, 250, 150);
			break;
		case PLAYER_ATTACK_MASK:
			App->renderer->DrawQuad((*it)->rect, 255, 250, 250, 200);
			break;
		case ENEMY_MASK:
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 150);
			break;
		case ENEMY_ATTACK_MASK:
			App->renderer->DrawQuad((*it)->rect, 255, 0, 0, 200);
			break;
		case OBSTACLE_MASK:
			App->renderer->DrawQuad((*it)->rect, 0, 250, 0, 150);
			break;
		case WALL_MASK:
			App->renderer->DrawQuad((*it)->rect, 0, 250, 0, 200);
			break;
		default:
			App->renderer->DrawQuad((*it)->rect, 255, 250, 250, 150);
			break;
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

Collider* ModuleCollision::AddCollider(const SDL_Rect& rect, const CollisionMask collisionMask, std::function<void()> OnCollisionCallback)
{
	Collider* ret = new Collider(rect, OnCollisionCallback);
	ret->collisionMask = collisionMask;

	colliders.push_back(ret);

	return ret;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& otherRect) const
{
	// TODO 7: Create by hand (avoid consulting the internet) a simple collision test
	// Return true if the argument and the own rectangle are intersecting
	
	bool ret = true;

	int LWall = rect.x;
	int RWall = rect.x + rect.w;
	int TWall = rect.y;
	int DWall = rect.y + rect.h;

	int otherLWall = otherRect.x;
	int otherRWall = otherRect.x + otherRect.w;
	int otherTWall = otherRect.y;
	int otherDWall = otherRect.y + otherRect.h;


	if ((LWall > otherRWall) || (RWall < otherLWall))
		ret = false;
	else 
	{
		if ((DWall < otherTWall) || (TWall > otherDWall))
			ret = false;
	}
	return ret;
}


void Collider::OnCollisionTrigger(const Collider & otherCollider) const
{
	
	App->renderer->DrawQuad(rect, 255, 250, 250, 255);
	App->renderer->DrawQuad(otherCollider.rect, 255, 250, 250, 255);

	if (this->OnCollisionCallback != nullptr)
		this->OnCollisionCallback();
	if (otherCollider.OnCollisionCallback != nullptr)
		otherCollider.OnCollisionCallback();



}
