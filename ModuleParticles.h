#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Animation.h"
#include "Point.h"
#include "ModuleCollision.h"

struct SDL_Texture;

struct Particle
{
	bool to_delete = false;

	// TODO 1: Fill in the structure with all the properties you need for each particle
	Animation animation;
	iPoint location;
	int depth;
	iPoint direction;
	int framesLive;
	int framesAlive;
	unsigned int audioFxIndex;
	
	// TODO 11: Add an optional collider to each particle
	Collider *collider = nullptr;
	CollisionMask collisionMask = DEFAULT_MASK;


	Particle();
	Particle(const Particle& p);
	~Particle();
	bool Update();
	void OnCollisionTrigger();
};

class ModuleParticles : public Module
{
public:
	ModuleParticles();
	~ModuleParticles();

	bool Start();
	update_status Update();
	bool CleanUp();

	void AddParticle(const Particle& particle, iPoint& location); // feel free to expand this call

private:

	SDL_Texture* graphics = nullptr;
	std::list<Particle*> active;

public:
	Particle laserShot;
	Particle explosion;

	// prototype particles go here ...
};

#endif // __MODULEPARTICLES_H__