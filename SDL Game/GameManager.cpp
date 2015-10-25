

#include "GameManager.h"

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <stdio.h>






SDL_Surface* GameManager::initSDL(char *t, int hres, int vres, int bpp)
{
	SDL_Surface *screen;

	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
		exit(1);
	SDL_WM_SetCaption(title, NULL);
	screen = SDL_SetVideoMode(hres, vres, bpp, SDL_FULLSCREEN|SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

	Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
	TTF_Init();
	return screen;
}


void GameManager::controlFPS()
{
	if( nextTick > SDL_GetTicks() )
		SDL_Delay( nextTick - SDL_GetTicks() );
	nextTick = SDL_GetTicks() + interval;
}


bool GameManager::impactCheck(SDL_Rect &objA, SDL_Rect &objB )
{
	if( objB.x <= objA.x + objA.w && objB.x + objB.w >= objA.x &&
		objB.y <= objA.y + objA.h && objB.y + objB.h >= objA.y)
		return true;
	else
		return false;
}










GameManager::GameManager(char *t, int hRes, int vRes, int bpp, int fps)
{
	atexit(SDL_Quit);

	sprintf(title, "%s", t);
	horizontalResolution = hRes;
	verticalResolution = vRes;
	bitsPerPixel = bpp;
	FPS = fps;
	interval = 1000/FPS;
	nextTick = 0;

	screen = initSDL(title, horizontalResolution, verticalResolution, bitsPerPixel);
	background = NULL;

	maxObjects = MAXOBJECTS;
	nObjects = 0;

	displayFunction = NULL;
	idleFunction = NULL;
	eventFunction = NULL;
	collisionFunction = NULL;

	gameRunning = false;
	musicPlaying = false;
	music = NULL;

	post = false;
	counter = 0;
	target = -1;

	objectsRoot = NULL;
	objectsTop = NULL;
}


GameManager::~GameManager()
{
	Mix_FreeMusic(music);

	GameObject* ptr;
	GameObject* temp;

	ptr = objectsRoot;
	while( ptr != NULL )
	{
		temp = ptr->next;
		delete ptr;
		ptr = temp;
	}
}









void GameManager::setBackgroundImage(char *fname)
{
	char imageName[200];

	if( background != NULL )
		SDL_FreeSurface(background);

	sprintf(imageName, "GameData\\%s",fname);
	background = IMG_Load(imageName);
}


void GameManager::setBackgroundMusic( char *fname)
{
	char musicName[200];

	if( music != NULL )
	{
		if( musicPlaying == true )
			Mix_PauseMusic();
		Mix_FreeMusic(music);
	}

	sprintf(musicName, "GameData\\%s",fname);
	music = Mix_LoadMUS(musicName);
	if( musicPlaying == true )
		playMusic();
}


void GameManager::playMusic()
{
	Mix_PlayMusic(music,-1);
	musicPlaying = true;
}


GameObject* GameManager::drawString(char *str, int x, int y, int r, int g, int b, int size)
{
	TTF_Font *font = TTF_OpenFont("GameData\\theFont.ttf", size);
	SDL_Color fontColor;

	fontColor.r = r;
	fontColor.g = g;
	fontColor.b = b;

	SDL_Surface *surface = TTF_RenderText_Solid( font , str, fontColor );
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	return (registerObject( surface, rect));
}








GameObject* GameManager::registerObject(SDL_Surface *img, SDL_Rect pos)
{
	if( nObjects == 0 )
	{
		objectsRoot = new GameObject;
		objectsTop = objectsRoot;
		objectsTop->image = img;
		objectsTop->position = pos;
		objectsTop->next = NULL;
		nObjects++;
	}
	else
	{
		objectsTop->next = new GameObject;
		objectsTop = objectsTop->next;
		objectsTop->image = img;
		objectsTop->position = pos;
		objectsTop->next = NULL;
		nObjects++;
	}
	return objectsTop;
}


void GameManager::removeObject(GameObject* objPtr)
{
	if( nObjects > 0 )
	{
		if( objPtr == objectsRoot )
		{
			if( objectsTop == objectsRoot )
				objectsTop = NULL;
			objectsRoot = objectsRoot->next;
			delete objPtr;
			nObjects--;
		}
		else
		{
			GameObject* ptr = objectsRoot;
			while( ptr->next!=objPtr && ptr->next!=NULL )
				ptr = ptr->next;

			ptr->next = objPtr->next;
			delete objPtr;
			nObjects--;
		}
	}
}




void GameManager::resetGame()
{
	if( nObjects > 0 )
	{
		GameObject* ptr = objectsRoot;
		objectsTop = objectsRoot;
		while( objectsTop->next != NULL )
		{
			ptr = objectsTop;
			objectsTop = objectsTop->next;
			delete ptr;
		}
		objectsRoot = NULL;
		nObjects = 0;
	}
	background = NULL;
	maxObjects = MAXOBJECTS;

	displayFunction = NULL;
	idleFunction = NULL;
	eventFunction = NULL;
	collisionFunction = NULL;
	music = NULL;

	post = false;
	counter = 0;
	target = -1;
}



void GameManager::updateObjectState(GameObject* objPtr, SDL_Surface *img, SDL_Rect pos)
{
	objPtr->image = img;
	objPtr->position = pos;
}









void GameManager::setDisplayFunction(void (*dispFn)())
{
	displayFunction = dispFn;
}

void GameManager::setIdleFunction(void (*idleFn)())
{
	idleFunction = idleFn;
}


void GameManager::setCollisionFunction(void (*collFn)(GameObject* objPtr1, GameObject* objPtr2))
{
	collisionFunction = collFn;
}


void GameManager::setEventFunction(void (*eventFn)(SDL_Event *))
{
	eventFunction = eventFn;
}









void GameManager::gameLoop()
{
	gameRunning = true;
	while( gameRunning )
	{
		GameObject* ptr = objectsRoot;
		if( post == true )
		{
			counter ++;
			if( counter == target )
			{
				SDL_Delay(5000);
				exit(1);
			}
		}

		if( background == NULL )
			SDL_FillRect(screen, NULL, 0x000000);
		else
			SDL_BlitSurface(background, NULL, screen, NULL);

		while( ptr!=NULL )
		{
				SDL_BlitSurface(ptr->image, NULL, screen, &ptr->position);
				ptr = ptr->next;
		}

		if( collisionFunction != NULL )
		{
			bool	collided;
			collided = impactCheck(objectsRoot->position, (objectsRoot->next)->position);
			if( collided == true )
				collisionFunction(objectsRoot, objectsRoot->next);
		}

		if( displayFunction != NULL )
			displayFunction();

		SDL_Flip(screen);
		controlFPS();

		if( SDL_PollEvent(&theEvent) )
		{
			switch( theEvent.type )
			{
				case SDL_QUIT:
					gameRunning = false;
					break;

				default:
					if( eventFunction != NULL )
						eventFunction(&theEvent);
					break;
			}
		}
		else if ( idleFunction != NULL )
			idleFunction();
	}
}







SDL_Surface* GameManager::getScreen()
{
	return screen;
}







void GameManager::Quit()
{
	gameRunning = false;
}

void GameManager::postQuit(int frames)
{
	if( post == false )
	{
		target = frames;
		post = true;
		counter = 0;
	}
}