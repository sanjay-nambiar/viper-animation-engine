#include "Character.h"

#include "SDL.h"
#include <stdio.h>



SDL_Surface* Character::flipSurface(SDL_Surface* flip_surface)
{
	SDL_Surface* copy_surface = NULL;

	// This only works for 32 bit pixel format
	if( flip_surface->format->BitsPerPixel == 32 )
	{
		// This surface must be freed by the caller.
		copy_surface = SDL_CreateRGBSurface(flip_surface->flags, flip_surface->w, flip_surface->h,
		flip_surface->format->BitsPerPixel,
		flip_surface->format->Rmask,
		flip_surface->format->Gmask,
		flip_surface->format->Bmask,
		flip_surface->format->Amask);
		Uint32 *source = static_cast<Uint32*>(flip_surface->pixels);
		Uint32 *dest = static_cast<Uint32*>(copy_surface->pixels);
		for(int i = 0; i < flip_surface->h; i++)
		{
			for(int j = 0; j < flip_surface->w; j++)
			{
				dest[ i * flip_surface->w + j ] = source[ (i + 1) * flip_surface->w - j];
			}
		}
	}
	return copy_surface;
}



Character::Character(GameManager *tG, char *folder)
{
	FILE		*fp;
	char		fname[500];
	SDL_Rect	drawPos;
	SDL_Rect	tempDel;

	sprintf(fname, "CharacterData\\%s\\%s.txt", folder, folder);

	fp = fopen(fname, "rt");
	fscanf(fp, "%s", name);
	fscanf(fp, "%d", &nActions);
	actions = new Action* [nActions];

	for(int i=0; i<nActions; i++)
		actions[i] = new Action(fp, folder);
	fscanf(fp, "%d", &defaultAction);
	defaultAction--;
	currentAction = defaultAction;
	fclose(fp);
	theGame = tG;
	position.x = 0;
	position.y = 0;
	position.h = actions[currentAction]->framesList[actions[currentAction]->currentFrame]->h;
	position.w = actions[currentAction]->framesList[actions[currentAction]->currentFrame]->w;
	drawPos = position;
	if( flipped == true )
	{
		tempDel.x = -(actions[currentAction]->delta.x);
		tempDel.y = (actions[currentAction]->delta.y);
		drawPos.x -= drawPos.w;
	}
	else
	{
		tempDel.x = (actions[currentAction]->delta.x);
		tempDel.y = (actions[currentAction]->delta.y);
	}
	drawPos.x += tempDel.x;
	drawPos.y += tempDel.y;
	charPtr = theGame->registerObject( actions[currentAction]->framesList[actions[currentAction]->currentFrame], drawPos);
	flipped = false;
}



Character::~Character()
{
	delete []actions;
}









void Character::toggleFlip()
{
	if( flipped == true )
		flipped = false;
	else
		flipped = true;
}








void Character::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
}


void Character::addToPosition(int x, int y)
{
	position.x += x;
	position.y += y;
}







void Character::setDefaultAction()
{
	currentAction = defaultAction;
	actions[currentAction]->loop = true;
	actions[currentAction]->initiateAction();
}

void Character::changeDefaultAction(int actionIndex)
{
	if( actionIndex > 0 && actionIndex <= nActions )
	{
		defaultAction = actionIndex;
		currentAction = defaultAction;
		actions[currentAction]->loop = true;
		actions[currentAction]->initiateAction();
	}
}

void Character::setAction(int actionIndex, bool loop)
{
	if( actionIndex > 0 &&  actionIndex <= nActions )
	{
		SDL_Rect	drawPos, tempDel;
		SDL_Surface *surface;

		actions[currentAction]->currentFrame = 0;
		currentAction = actionIndex-1;
		actions[currentAction]->loop = loop;
		actions[currentAction]->initiateAction();

		position.h = actions[currentAction]->framesList[actions[currentAction]->currentFrame]->h;
		position.w = actions[currentAction]->framesList[actions[currentAction]->currentFrame]->w;
		drawPos = position;
		if( flipped == true )
		{
			tempDel.x = -(actions[currentAction]->delta.x);
			tempDel.y = (actions[currentAction]->delta.y);
			drawPos.x -= drawPos.w;
		}
		else
		{
			tempDel.x = (actions[currentAction]->delta.x);
			tempDel.y = (actions[currentAction]->delta.y);
		}
		drawPos.x += tempDel.x;
		drawPos.y += tempDel.y;

		surface = actions[currentAction]->framesList[actions[currentAction]->currentFrame];
		if( flipped == true )
			surface = flipSurface(surface);
		theGame->updateObjectState(charPtr, surface, drawPos);
	}
}


void Character::playAction()
{
	SDL_Rect	drawPos, tempDel;
	SDL_Surface *surface;

	if( actions[currentAction]->updateState() == -1 )
		currentAction = defaultAction;

	position.h = actions[currentAction]->framesList[actions[currentAction]->currentFrame]->h;
	position.w = actions[currentAction]->framesList[actions[currentAction]->currentFrame]->w;
	drawPos = position;
	if( flipped == true )
	{
		tempDel.x = -(actions[currentAction]->delta.x);
		tempDel.y = (actions[currentAction]->delta.y);
		drawPos.x -= drawPos.w;
	}
	else
	{
		tempDel.x = (actions[currentAction]->delta.x);
		tempDel.y = (actions[currentAction]->delta.y);
	}
	drawPos.x += tempDel.x;
	drawPos.y += tempDel.y;

	surface = actions[currentAction]->framesList[actions[currentAction]->currentFrame];
	if( flipped == true )
		surface = flipSurface(surface);
	theGame->updateObjectState(charPtr, surface, drawPos);
}








char* Character::getName()
{
	return name;
}

int Character::getCurrentAction()
{
	return currentAction;
}

int Character::getCurrentActionFramesNo()
{
	return actions[currentAction]->nFrames;
}