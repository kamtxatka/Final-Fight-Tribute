#ifndef __MODULERENDER_H__
#define __MODULERENDER_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"

#define CAMERA_SPEED 4

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

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
	bool DrawQuad(const SDL_Rect& rect, const int& z, const int& depth, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	bool BlitStretch(SDL_Texture* texture, SDL_Rect* target, SDL_Rect* section, float speed = 1.0f);

public:
	SDL_Renderer* renderer = nullptr;
	SDL_Rect camera;
};

#endif // __MODULERENDER_H__