#include <math.h>
#include "ModuleParticles.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollision.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{}

ModuleParticles::~ModuleParticles()
{}

// Load assets
bool ModuleParticles::Start()
{
	LOG("Loading particles");
	graphics = App->textures->Load("rtype/particles.png");


	// TODO 2: Create a prototype for the laser particle
	// audio: rtype/laser.wav
	// coords: {232, 103, 16, 12}; {249, 103, 16, 12};
	laserShot.animation.frames.push_back({ 232, 103, 16, 12 });
	laserShot.animation.frames.push_back({ 249, 103, 16, 12 });
	laserShot.location = { 100, 100, 0 };
	laserShot.depth = 5;
	laserShot.direction = { 5, 0 };
	laserShot.framesAlive = 0;
	laserShot.framesLive = 40;
	laserShot.audioFxIndex = App->audio->LoadFx("rtype/laser.wav");
	laserShot.collider = nullptr;
	laserShot.collisionMask = PLAYER_ATTACK_MASK;


	// TODO 12: Create a new "Explosion" particle 
	// audio: rtype/explosion.wav
	// coords: {274, 296, 33, 30}; {313, 296, 33, 30}; {346, 296, 33, 30}; {382, 296, 33, 30}; {419, 296, 33, 30}; {457, 296, 33, 30};
	explosion.animation.frames.push_back({ 274, 296, 33, 30 });
	explosion.animation.frames.push_back({ 313, 296, 33, 30 });
	explosion.animation.frames.push_back({ 346, 296, 33, 30 });
	explosion.animation.frames.push_back({ 382, 296, 33, 30 });
	explosion.animation.frames.push_back({ 419, 296, 33, 30 });
	explosion.animation.frames.push_back({ 457, 296, 33, 30 });
	explosion.location = { 50, 100 };
	explosion.depth = 5;
	explosion.direction = { 0, 0 };
	explosion.framesAlive = 0;
	explosion.framesLive = 20;
	explosion.audioFxIndex = App->audio->LoadFx("rtype/explosion.wav");
	explosion.collider = nullptr;
	explosion.collisionMask = PLAYER_ATTACK_MASK;


	return true;
}

// Unload assets
bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");
	App->textures->Unload(graphics);

	for (list<Particle*>::iterator it = active.begin(); it != active.end(); ++it)
		RELEASE(*it);

	active.clear();

	return true;
}

// Update: draw background
update_status ModuleParticles::Update()
{
	for (list<Particle*>::iterator it = active.begin(); it != active.end();)
	{
		Particle* p = *it;

		if(p->Update() == false)
		{
			RELEASE(*it);
			it = active.erase(it);
		}
		else
		{
			//App->renderer->Blit(graphics, p->location, &(p->animation.GetCurrentFrame()), 1);
			App->renderer->AddBlitCall(graphics, p->location, &(p->animation.GetCurrentFrame()), 1);

			++it;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& prefab, iPoint& location)
{
	// TODO 4: Fill in a method to create an instance of a prototype particle	

	Particle* p = new Particle(prefab);
	p->location = location;

	//una vez añadida le ponemos la particula. w y h seran los de la animacion pero la x y hay que ponerl la misma que la particula
	p->collider = App->collision->AddCollider({ p->animation.GetCurrentFrame() }, p->location.z, p->depth,
		p->collisionMask, std::bind(&Particle::OnCollisionTrigger, p));

	p->collider->rect.x = p->location.x + p->direction.x;
	p->collider->rect.y = p->location.y + p->direction.y;
	p->collider->z = p->location.z + p->direction.z;

	active.push_back(p);
}

// -------------------------------------------------------------
// -------------------------------------------------------------

Particle::Particle()
{}

// TODO 3: Fill in a copy constructor
Particle::Particle(const Particle& p) 
{
	this->animation = p.animation;
	//this->location = p.location;
	this->depth = p.depth;
	this->direction = p.direction;
	this->framesLive = p.framesLive;
	this->framesAlive = 0;
	this->audioFxIndex = p.audioFxIndex;
	this->collisionMask = p.collisionMask;
}

Particle::~Particle()
{
}

bool Particle::Update()
{
	bool ret = true;

	// TODO 5: This is the core of the particle functionality.
	// Return false if the particle must be destroyed

	if (this->framesAlive == 0)
	{
		App->audio->PlayFx(audioFxIndex);
	}

	if (this->framesAlive >= this->framesLive)
	{
		if (collider != nullptr)
			collider->to_delete = true;

		to_delete = true;
		ret = false;
	}
	location += direction;
	framesAlive++;

	//actualizamos la posicion del collider junto con la de la particula
	if (collider != nullptr)
	{
		collider->rect.x = this->location.x + direction.x;
		collider->rect.y = this->location.y + direction.y;
		collider->z = this->location.z + direction.z;
	}

	return ret;
}

void Particle::OnCollisionTrigger()
{
	LOG("particle colision");
	//App->collision->AddCollider({ SCREEN_WIDTH / 2 + 30, SCREEN_HEIGHT / 2 - 30, 10, 10 }, 0, 0, ENEMY_MASK, nullptr);
	this->collider->to_delete = true;
	this->to_delete = true;
}

