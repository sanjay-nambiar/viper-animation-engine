#ifndef		GAME_MANAGER_CLASS
#define		GAME_MANAGER_CLASS



#include "SDL.h"
#include "SDL_mixer.h"



#define		MAXOBJECTS		100



struct GameObject
{
	SDL_Surface*	image;
	SDL_Rect		position;
	GameObject*		next;
};



class GameManager
{
	char			title[50];
	unsigned int	horizontalResolution;
	unsigned int	verticalResolution;
	unsigned int	bitsPerPixel;
	unsigned int	FPS;
	unsigned int	interval;
	unsigned int	nextTick;

	SDL_Surface		*screen;
	SDL_Surface		*background;
	int				maxObjects;
	int				nObjects;
	GameObject		*objectsRoot;
	GameObject		*objectsTop;

	bool			gameRunning;
	SDL_Event		theEvent;
	Mix_Music		*music;
	bool			musicPlaying;
	bool			post;
	int				counter, target;



	SDL_Surface	*initSDL(char *t, int hres, int vres, int bpp);
	void controlFPS();

	bool impactCheck(SDL_Rect &objA, SDL_Rect &objB );

	void (*displayFunction) ();
	void (*idleFunction) ();
	void (*eventFunction) (SDL_Event *evt);
	void (*collisionFunction) (GameObject* objPtr1, GameObject* objPtr2);


public:

	GameManager(char *t, int hRes, int vRes, int bpp, int fps);
	~GameManager();

	void setBackgroundImage(char *fname);
	void setBackgroundMusic(char *fname);

	void playMusic();
	GameObject* drawString(char *str, int x, int y, int r, int g, int b, int size);

	GameObject*  registerObject(SDL_Surface *img, SDL_Rect pos);
	void removeObject(GameObject* objPtr);
	void resetGame();
	void updateObjectState(GameObject* objID, SDL_Surface *img, SDL_Rect pos);

	void setDisplayFunction(void (*dispFn)());
	void setIdleFunction(void (*idleFn)());
	void setEventFunction(void (*eventFn)(SDL_Event *));
	void setCollisionFunction(void (*collFn)(GameObject* objPtr1, GameObject* objPtr2));

	SDL_Surface	*getScreen();

	void gameLoop();
	void Quit();
	void postQuit(int frames);
};


#endif