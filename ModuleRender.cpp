#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleRender::ModuleRender()
{
	camera.x = camera.y = 0;
	camera.w = SCREEN_WIDTH * SCREEN_SIZE;
	camera.h = SCREEN_HEIGHT* SCREEN_SIZE;
}

// Destructor
ModuleRender::~ModuleRender()
{}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");
	bool ret = true;
	Uint32 flags = 0;

	if(VSYNC == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->window->window, -1, flags);
	
	if(renderer == nullptr)
	{
		LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	return ret;
}


update_status ModuleRender::PreUpdate()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->renderer->camera.y += CAMERA_SPEED;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->renderer->camera.y -= CAMERA_SPEED;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->renderer->camera.x += CAMERA_SPEED;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->renderer->camera.x -= CAMERA_SPEED;

	//stackoverflow.com/questions/8121837/sorting-a-list-of-a-custom-type 
	//anachronism used warning... ¿?

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	// blits calls should be here after all the logic is done, except for backgrounds

	blitCallList.sort([](const BlitCall& a, const BlitCall& const b) { return a.location.z < b.location.z; });
	for (std::list<BlitCall>::iterator it = blitCallList.begin(); it != blitCallList.end(); ++it)
	{
		Blit(it->texture, it->location, it->section, it->speed);
	}

	blitCallList.clear();

	SDL_RenderPresent(renderer);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window
	if(renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}

	return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, iPoint coordinates, SDL_Rect* section, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + coordinates.x * SCREEN_SIZE;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

	rect.y = (int)(camera.y * speed) + (coordinates.y - rect.h + coordinates.z) * SCREEN_SIZE;

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ModuleRender::BlitStretch(SDL_Texture * texture, SDL_Rect * target, SDL_Rect * section, float speed)
{
	bool ret = true;
	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + target->x * SCREEN_SIZE;
	rect.y = (int)(camera.y * speed) + target->y * SCREEN_SIZE;

	if (section != NULL)
	{
		/*rect.w = section->w;
		rect.h = section->h;*/
		rect.w = target->w;
		rect.h = target->h;

	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= SCREEN_SIZE;
	rect.h *= SCREEN_SIZE;

	if (SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void ModuleRender::AddBlitCall(SDL_Texture * texture, iPoint location, SDL_Rect * section, float speed)
{
	blitCallList.push_back(BlitCall(texture, location, section, speed));
}


// it prints 2 quads. a rectangle on min Z of the collider and another rectangle on max Z of the collider
// (0,0,0) of a cube is on its top, left, back corner
bool ModuleRender::DrawQuad(const SDL_Rect& rect, const int& z, const int& depth, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);

	rec.x = (int)(camera.x + rect.x * SCREEN_SIZE);
	rec.y = (int)(camera.y + (rect.y - rect.h + z) * SCREEN_SIZE);

	int recty2 = (int)(camera.y + (rect.y - rect.h + z - depth) * SCREEN_SIZE);
	rec.w *= SCREEN_SIZE;
	rec.h *= SCREEN_SIZE;


	if (SDL_RenderFillRect(renderer, &rec) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}
	//first quad drawn

	//second quad goes on max Z so we have to add depth

	/*rec.y = (int)(camera.y + (rect.y + z - depth) * SCREEN_SIZE);*/

	SDL_Rect rect2 = { rec.x, recty2,rec.w,rec.h };


	if (SDL_RenderFillRect(renderer, &rect2) != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}



	return ret;
}

