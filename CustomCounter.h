#ifndef __CUSTOM_COUNTER_H__
#define __CUSTOM_COUNTER_H__


#include "SDL/include/SDL.h"


class CustomCounter
{

public:
	bool isPaused = false;
	bool isStarted = false;;

private:
	Uint32 startTicks = 0;
	Uint32 storedTicks = 0;

public:
	CustomCounter()
	{
		startTicks = 0;
		storedTicks = 0;
		isPaused = 0;
		isStarted = 0;
	}

	void Start()
	{
		isStarted = true;
		isPaused = false;
		startTicks = SDL_GetTicks();
		storedTicks = 0;
	}

	void Stop()
	{
		isStarted = false;
		isPaused = false;
		startTicks = 0;
		storedTicks = 0;
	}

	void Pause()
	{
		if (isStarted && !isPaused)
		{
			isPaused = true;
			storedTicks = SDL_GetTicks() - startTicks;
			startTicks = 0;
		}
	}

	void Unpause()
	{
		if (isStarted && isPaused)
		{
			isPaused = false;
			startTicks = SDL_GetTicks() - storedTicks;
			storedTicks = 0;
		}
	}

	Uint32 GetTime()
	{
		Uint32 time = 0;

		if (isStarted)
		{
			if (isPaused)
				time = storedTicks;
			else
				time = SDL_GetTicks() - startTicks;
		}
		return time;
	}

};




#endif
