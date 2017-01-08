#ifndef __MODULESCENES1_H__
#define __MODULESCENES1_H__

#include "Module.h"


	
class ModuleScene1_1 :	public Module
{
public:
	ModuleScene1_1(bool active = true);
	~ModuleScene1_1();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* backgroundgraphics = nullptr;
	SDL_Texture* skygraphics = nullptr;

	SDL_Rect background;
	SDL_Rect sky;
	SDL_Rect skyBlit1;
	SDL_Rect skyBlit2;
	SDL_Rect skyBlit3;




};

#endif
