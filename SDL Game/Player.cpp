
#include "Player.h"
#include "GameManager.h"


Player::Player(char *name, GameManager *tG)
{
	theCharacter = new Character( tG, name);
	life = 100.0;
	power = 0;
}


Player::~Player()
{
	delete theCharacter;
}

