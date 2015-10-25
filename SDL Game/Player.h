#ifndef		PLAYER_CLASS
#define		PLAYER_CLASS


#include "Character.h"


class	Player
{
public:

	Character*	theCharacter;
	float		life;
	int			power;

	Player(char *name, GameManager *tG);
	~Player();
};


#endif