#ifndef __MODULEOBSTACLE__
#define __MODULEOBSTACLE__

#include <list>
#include "Module.h"




struct Obstacle
{
	bool to_delete = false;
	bool destructible = false;
	iPoint location = { 0,0 };
	SDL_Rect rect = { 0,0,0,0 };
	Collider* collider = nullptr;
	CollisionMask collisionMask = OBSTACLE_MASK;


	Obstacle();
	Obstacle(const Obstacle& p);
	~Obstacle();
	bool Update();
	void OnCollisionTrigger();
};


class ModuleObstacle :	public Module
{
public:
	ModuleObstacle();
	~ModuleObstacle();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddObstacle(const Obstacle& obstacle, iPoint location);

private:
	SDL_Texture* graphics = nullptr;
	std::list<Obstacle*> active;

public:
	Obstacle barrrel;





	SDL_Rect barrel;
};


#endif
