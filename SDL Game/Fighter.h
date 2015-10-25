#ifndef		FIGHTER_CLASS
#define		FIGHTER_CLASS


#include "GameManager.h"
#include "Character.h"
#include "Player.h"


#define		TITLE			"SDL Window"
#define		HRES			640
#define		VRES			480
#define		BPP				32
#define		FPS				20
#define		NUM_PLAYERS		2



class Fighter
{
	GameManager*	theGame;
	int				availablePlayers;
	char**			playerNames;
	int				nPlayers;
	Player*			players[NUM_PLAYERS];
	int				option;
	SDL_Surface*	selection;
	SDL_Rect		selectionPos;
	Mix_Chunk*		selectSound;
	GameObject*		selectionPtr;

	int				currentPlayers[2];
	SDL_Surface**	charImages;
	SDL_Rect*		charPositions;
	GameObject**	charPtrs;
	

	static void displayFunction();
	static void collisionFunction(GameObject*, GameObject*);
	static void idleFunction();

	static void menuEventFunction(SDL_Event*);
	static void charSelectEventFunction(SDL_Event*);
	static void fightEventFunction(SDL_Event*);

public:

	enum	Actions{ ENTRY=1, STANCE, WALK, BLOCK, MP, HP, MK, HK, 
			SPECIAL1, SPECIAL2, SUPER1, SUPER2, WINS, FALLS};


	Fighter(char *fname);
	~Fighter();

	void addPlayers(int pl);
	void removePlayer();
	void startGame();
	void showMenu();
	void showCharacterSelect();
	void startFight();
};

extern	Fighter*	theFighter;

#endif