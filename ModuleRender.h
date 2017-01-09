#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include <list>
#include "Module.h"
#include "Globals.h"
#include "Point.h"

#define CAMERA_SPEED 4

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;



struct BlitCall
{
	SDL_Texture* texture = nullptr;
	iPoint location = { 0,0,0 };
	SDL_Rect* section = nullptr;
	float speed = 0;

	BlitCall(SDL_Texture* texture, iPoint location, SDL_Rect* section, float speed = 1.0f) :
		texture(texture), location(location), section(section), speed(speed) {}
};



class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	bool Blit(SDL_Texture* texture, iPoint coordinates, SDL_Rect* section, float speed = 1.0f);
	bool BlitStretch(SDL_Texture* texture, SDL_Rect* target, SDL_Rect* section, float speed = 1.0f);
	void AddBlitCall(SDL_Texture* texture, iPoint location, SDL_Rect* section, float speed = 1.0f);
	bool DrawQuad(const SDL_Rect& rect, const int& z, const int& depth, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
	std::list<BlitCall> blitCallList;
};

#endif // __MODULERENDER_H__