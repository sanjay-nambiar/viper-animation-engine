
#include "Action.h"

#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_mixer.h"
#include <stdio.h>



Action::Action(FILE	*fp, char *folder)
{
	char	imageName[500];
	char	temp[100];

	fscanf(fp, "%d", &nFrames);
	framesList = new SDL_Surface* [nFrames];

	fscanf(fp, "%d%d", &delta.x, &delta.y);

	fscanf(fp, "%s", temp);
	sprintf(imageName, "CharacterData\\%s\\%s", folder, temp);
	wav = Mix_LoadWAV(imageName);
	fscanf(fp, "%d", &loopMusic);

	for(int i=0; i<nFrames; i++)
	{
		fscanf(fp, "%s", temp);
		sprintf(imageName, "CharacterData\\%s\\%s", folder, temp);
		framesList[i] = IMG_Load(imageName);
	}
	currentFrame = 0;
}


Action::~Action()
{
	for(int i=0; i<nFrames; i++)
		SDL_FreeSurface(framesList[i]);
	delete [] framesList;
	Mix_FreeChunk(wav);
}









void Action::initiateAction()
{
	Mix_PlayChannel(-1,wav,loopMusic);
}


int Action::updateState()
{
	if( loop == true )
		currentFrame = ( currentFrame + 1 ) % nFrames;
	else
	{
		currentFrame = currentFrame++;
		if( currentFrame >= nFrames )
		{
			currentFrame = 0;
			return -1;
		}
	}
	return 0;
}
