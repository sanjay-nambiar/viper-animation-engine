#ifndef		ACTION_CLASS
#define		ACTION_CLASS


#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_mixer.h"


class Action
{
public:
	int				nFrames;
	SDL_Surface		**framesList;
	int				currentFrame;
	bool			loop;
	SDL_Rect		delta;
	Mix_Chunk		*wav;
	int				loopMusic;


	Action(FILE	*fp, char *folder);
	~Action();

	void initiateAction();
	int updateState();
};



#endif
