#ifndef		CHARACTER_CLASS
#define		CHARACTER_CLASS


#include "SDL.h"
#include "GameManager.h"
#include "Action.h"



class Character
{
	GameObject*			charPtr;
	char				name[100];
	SDL_Rect			position;
	SDL_Rect			drawPosition;
	int					nActions;
	Action**			actions;
	int					currentAction;
	int					defaultAction;
	GameManager*		theGame;
	bool				flipped;


	SDL_Surface* flipSurface(SDL_Surface* flip_surface);


public:

	Character(GameManager *tG, char *folder);
	~Character();

	void toggleFlip();

	void setPosition(int x, int y);
	void addToPosition(int x, int y);

	void setDefaultAction();
	void changeDefaultAction(int actionIndex);
	void setAction(int actionIndex, bool loop = false);
	void playAction();

	char* getName();
	int   getCurrentAction();
	int   getCurrentActionFramesNo();
};

#endif