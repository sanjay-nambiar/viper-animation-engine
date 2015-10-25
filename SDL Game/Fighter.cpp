
#include "Fighter.h"
#include <stdio.h>
#include <stdlib.h>



Fighter*	theFighter = NULL;



Fighter::Fighter(char *fname)
{
	FILE *fp;
	char datName[100];

	sprintf(datName,"GameData\\%s", fname);
	fp = fopen(datName, "rt");
	fscanf(fp, "%d", &availablePlayers);
	playerNames = new char* [availablePlayers];
	for(int i=0; i<availablePlayers; i++)
	{
		playerNames[i] = new char [50];
		fscanf(fp, "%s", playerNames[i]);
	}

	theGame = new GameManager(TITLE, HRES, VRES, BPP, FPS);
	nPlayers = 0;
	fclose(fp);

	charImages = new SDL_Surface* [availablePlayers];
	charPositions = new SDL_Rect [availablePlayers];
	charPtrs = new GameObject* [availablePlayers];

	for(int i=0; i<availablePlayers; i++)
	{
		sprintf(datName, "CharacterData\\%s\\%s.png", playerNames[i], playerNames[i]);
		charImages[i] = IMG_Load(datName);
		charPositions[i].x = 94 + ((i%3)*150);
		charPositions[i].y = 14 + ((i/3)*150);
	}

	currentPlayers[0] = 0;
	currentPlayers[1] = 1;
	selectSound = NULL;
	selectionPtr = NULL;
	selection = NULL;
}


Fighter::~Fighter()
{
	for(int i = 0; i<theFighter->availablePlayers; i++)
		SDL_FreeSurface(theFighter->charImages[i]);

	delete [] charImages;
	delete [] charPositions;
	delete [] charPtrs;

	for(int i=0; i<nPlayers; i++)
		delete players[i];

	delete [] players;
	delete theGame;
}



void Fighter::addPlayers(int pl)
{
	if( pl>=0 && pl<availablePlayers && nPlayers<NUM_PLAYERS)
	{
		players[nPlayers] = new Player(playerNames[pl], theGame);
		nPlayers++;
	}
}



void Fighter::removePlayer()
{
	if( nPlayers > 0 )
		nPlayers--;
	delete players[nPlayers];
}





void Fighter::displayFunction()
{
	for(int i=0; i<theFighter->nPlayers; i++)
		theFighter->players[i]->theCharacter->playAction();

	// life bars
	SDL_Rect	barRect1, barRect2;

	barRect1.x = 70;
	barRect1.y = 50;
	barRect1.w = 104;
	barRect1.h = 14;

	barRect2.x = 470;
	barRect2.y = 50;
	barRect2.w = 104;
	barRect2.h = 14;
	
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect1, SDL_MapRGB(theFighter->theGame->getScreen()->format , 255 , 255 , 255 ) );
	barRect1.x += 2;
	barRect1.y += 2;
	barRect1.w -= 4;
	barRect1.h -= 4;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect1, SDL_MapRGB(theFighter->theGame->getScreen()->format , 0 , 0 , 0 ) );
	barRect1.w = (int)theFighter->players[0]->life;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect1, SDL_MapRGB(theFighter->theGame->getScreen()->format , 255 , 0 , 0 ) );

	SDL_FillRect( theFighter->theGame->getScreen(), &barRect2, SDL_MapRGB(theFighter->theGame->getScreen()->format , 255 , 255 , 255 ) );
	barRect2.x += 2;
	barRect2.y += 2;
	barRect2.w -= 4;
	barRect2.h -= 4;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect2, SDL_MapRGB(theFighter->theGame->getScreen()->format , 0 , 0 , 0 ) );
	barRect2.w = (int)theFighter->players[1]->life;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect2, SDL_MapRGB(theFighter->theGame->getScreen()->format , 255 , 0 , 0 ) );


	// power bars
	barRect1.x = 70;
	barRect1.y = 64;
	barRect1.w = 104;
	barRect1.h = 8;

	barRect2.x = 470;
	barRect2.y = 64;
	barRect2.w = 104;
	barRect2.h = 8;

	SDL_FillRect( theFighter->theGame->getScreen(), &barRect1, SDL_MapRGB(theFighter->theGame->getScreen()->format , 255 , 255 , 255 ) );
	barRect1.x += 2;
	barRect1.y += 2;
	barRect1.w -= 4;
	barRect1.h -= 4;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect1, SDL_MapRGB(theFighter->theGame->getScreen()->format , 0 , 0 , 0 ) );
	barRect1.w = theFighter->players[0]->power;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect1, SDL_MapRGB(theFighter->theGame->getScreen()->format , 0 , 255 , 0 ) );
	
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect2, SDL_MapRGB(theFighter->theGame->getScreen()->format , 255 , 255 , 255 ) );
	barRect2.x += 2;
	barRect2.y += 2;
	barRect2.w -= 4;
	barRect2.h -= 4;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect2, SDL_MapRGB(theFighter->theGame->getScreen()->format , 0 , 0 , 0 ) );
	barRect2.w = theFighter->players[1]->power;
	SDL_FillRect( theFighter->theGame->getScreen(), &barRect2, SDL_MapRGB(theFighter->theGame->getScreen()->format , 0 , 255 , 0 ) );
}











void Fighter::collisionFunction(GameObject* objPtr1, GameObject* objPtr2)
{
	if( theFighter->players[0]->theCharacter->getCurrentAction() >= Fighter::MP && 
		theFighter->players[1]->life > 0 )
	{
		theFighter->players[1]->life -= 0.2f;
		if( theFighter->players[0]->power < 100 )
			theFighter->players[0]->power += 1;
	}
	if( theFighter->players[1]->theCharacter->getCurrentAction() >= Fighter::MP &&
		theFighter->players[0]->life > 0 )
	{
		theFighter->players[0]->life -= 0.2f;
		if( theFighter->players[1]->power < 100 )
			theFighter->players[1]->power += 1;
	}

	if( theFighter->players[1]->life <= 0.0 )
	{
		char str[100];
		strcpy( str, theFighter->players[0]->theCharacter->getName());
		strcat( str, " Wins");
		theFighter->theGame->drawString(str, 190, 200, 255, 255, 255, 48);
		theFighter->players[0]->theCharacter->setAction( Fighter::WINS );
		theFighter->players[1]->theCharacter->setAction( Fighter::FALLS );
		theFighter->theGame->setEventFunction(NULL);
		theFighter->theGame->postQuit(30);//theCharacter.getCurrentActionFramesNo());
	}
	else if( theFighter->players[0]->life <= 0.0 )
	{
		char str[100];
		strcpy( str, theFighter->players[1]->theCharacter->getName());
		strcat( str, " Wins");
		theFighter->theGame->drawString(str, 190, 200, 255, 255, 255, 48);
		theFighter->players[1]->theCharacter->setAction( Fighter::WINS );
		theFighter->players[0]->theCharacter->setAction( Fighter::FALLS );
		theFighter->theGame->setEventFunction(NULL);
		theFighter->theGame->postQuit(30);//theCharacter2.getCurrentActionFramesNo());
	}
}












void Fighter::idleFunction()
{
	static int	i = -1;
	static int  action = Fighter::STANCE;
	static GameObject*	readyPtr;

	if( theFighter->players[1]->life > 0 )
	{
		if( i == -1)
		{
			static int init = 0;
			init ++;
			if( init >= theFighter->players[0]->theCharacter->getCurrentActionFramesNo() )
			{
				i = 0;
				//theFighter->theGame->removeObject(readyPtr);
			}/*
			if( init == 0 )
				readyPtr = theFighter->theGame->drawString("GET READY", 250, 200, 255, 255, 255, 48);*/
		}
		else if( i >= theFighter->players[1]->theCharacter->getCurrentActionFramesNo() )
		{
			action = Fighter::MP + ( rand()% (Fighter::WINS-Fighter::MP) );
			if( theFighter->players[1]->power>=90 )
			{
				theFighter->players[1]->theCharacter->setAction(action);
				if( action == Fighter::SUPER1 || action == Fighter::SUPER2 )
					theFighter->players[1]->power  -= 90;
				else if( action == Fighter::SPECIAL1 || action == Fighter::SPECIAL2 )
					theFighter->players[1]->power -= 40;
			}
			else if( theFighter->players[1]->power>=40 )
			{
				if( action != Fighter::SUPER1 && action!=Fighter::SUPER2 )
				{
					theFighter->players[1]->theCharacter->setAction(action);
					if( action == Fighter::SPECIAL1 || action == Fighter::SPECIAL2 )
						theFighter->players[1]->power -= 40;
				}
			}
			else if( action < Fighter::SPECIAL1 )
				theFighter->players[1]->theCharacter->setAction(action);
			i = 0;
		}
		else
			i++;
	}
}











void Fighter::menuEventFunction(SDL_Event* evt)
{
	switch( evt->type )
	{
		case SDL_KEYDOWN:
			switch( evt->key.keysym.sym )
			{
				case SDLK_UP:
					if( theFighter->option > 0 )
					{
						theFighter->option--;
						theFighter->selectionPos.y -= 55;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else
					{
						theFighter->option = 2;
						theFighter->selectionPos.y += 110;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					Mix_PlayChannel(-1, theFighter->selectSound, 0);
					break;

				case SDLK_DOWN:
					if( theFighter->option < 2 )
					{
						theFighter->option++;
						theFighter->selectionPos.y += 55;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else
					{
						theFighter->option = 0;
						theFighter->selectionPos.y -= 110;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					Mix_PlayChannel(-1, theFighter->selectSound, 0);
					break;

				case SDLK_RETURN:
					if( theFighter->option == 0 )
					{
						theFighter->theGame->resetGame();
						theFighter->option = 0;
						theFighter->showCharacterSelect();
					}
					else if( theFighter->option == 1 )
					{
					}
					else if( theFighter->option == 2 )
						theFighter->theGame->Quit();
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}
}




void Fighter::charSelectEventFunction(SDL_Event* evt)
{
	switch( evt->type )
	{
		case SDL_KEYDOWN:
			switch( evt->key.keysym.sym )
			{
				case SDLK_UP:
					if( theFighter->option > 2 )
					{
						theFighter->option-=3;
						theFighter->selectionPos.y -= 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else if( (theFighter->option + 6) < theFighter->availablePlayers )
					{
						theFighter->option += 6;
						theFighter->selectionPos.y += 300;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else if( (theFighter->option + 3) < theFighter->availablePlayers )
					{
						theFighter->option += 3;
						theFighter->selectionPos.y += 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					Mix_PlayChannel(-1, theFighter->selectSound, 0);
					break;

				case SDLK_DOWN:
					if( (theFighter->option < 6) && ((theFighter->option+3) < theFighter->availablePlayers))
					{
						theFighter->option+=3;
						theFighter->selectionPos.y += 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else
					{
						theFighter->option = theFighter->option % 3;
						theFighter->selectionPos.y = 12;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					Mix_PlayChannel(-1, theFighter->selectSound, 0);
					break;

				case SDLK_LEFT:
					if( (theFighter->option%3) > 0 )
					{
						theFighter->option--;
						theFighter->selectionPos.x -= 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else if( theFighter->option > 0 )
					{
						theFighter->option--;
						theFighter->selectionPos.x += 300;
						theFighter->selectionPos.y -= 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else
					{
						theFighter->option = theFighter->availablePlayers - 1;
						theFighter->selectionPos.x = 92 + ((theFighter->option%3)*150);
						theFighter->selectionPos.y = 12 + ((theFighter->option/3)*150);
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					Mix_PlayChannel(-1, theFighter->selectSound, 0);
					break;

				case SDLK_RIGHT:
					if( (theFighter->option%3) < 2 && ((theFighter->option+1) < theFighter->availablePlayers))
					{
						theFighter->option++;
						theFighter->selectionPos.x += 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else if( theFighter->option < (theFighter->availablePlayers-1) )
					{
						theFighter->option ++;
						theFighter->selectionPos.x -= 300;
						theFighter->selectionPos.y += 150;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					else
					{
						theFighter->option = 0;
						theFighter->selectionPos.x = 92;
						theFighter->selectionPos.y = 12;
						theFighter->theGame->updateObjectState(theFighter->selectionPtr, theFighter->selection, theFighter->selectionPos);
					}
					Mix_PlayChannel(-1, theFighter->selectSound, 0);
					break;

				case SDLK_RETURN:
					theFighter->currentPlayers[0] = theFighter->option;
					theFighter->currentPlayers[1] = (rand() % theFighter->availablePlayers);
					theFighter->theGame->resetGame();
					theFighter->option = 0;
					theFighter->startFight();
					break;

				case SDLK_ESCAPE:
					theFighter->theGame->resetGame();
					theFighter->option = 0;
					theFighter->showMenu();
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}
}







void Fighter::fightEventFunction(SDL_Event *evt)
{
	switch( evt->type )
	{
		case SDL_KEYDOWN:
			switch( evt->key.keysym.sym )
			{
				case SDLK_a:
					theFighter->players[0]->theCharacter->setAction(Fighter::MP);
					break;

				case SDLK_s:
					theFighter->players[0]->theCharacter->setAction(Fighter::HP);
					break;

				case SDLK_d:
					theFighter->players[0]->theCharacter->setAction(Fighter::MK);
					break;

				case SDLK_f:
					theFighter->players[0]->theCharacter->setAction(Fighter::HK);
				break;

				case SDLK_z:
					if( theFighter->players[0]->power >= 40 )
					{
						theFighter->players[0]->theCharacter->setAction(Fighter::SPECIAL1);
						theFighter->players[0]->power -= 40;
					}
					break;

				case SDLK_x:
					if( theFighter->players[0]->power >= 40 )
					{
						theFighter->players[0]->theCharacter->setAction(Fighter::SPECIAL2);
						theFighter->players[0]->power -= 40;
					}
					break;

				case SDLK_c:
					if( theFighter->players[0]->power >= 90 )
					{
						theFighter->players[0]->theCharacter->setAction(Fighter::SUPER1);
						theFighter->players[0]->power -= 90;
					}
					break;

				case SDLK_v:
					if( theFighter->players[0]->power >= 90 )
					{
						theFighter->players[0]->theCharacter->setAction(Fighter::SUPER2);
						theFighter->players[0]->power -= 90;
					}
					break;

				case SDLK_LEFT:
					theFighter->players[0]->theCharacter->addToPosition(-20, 0);
					theFighter->players[0]->theCharacter->setAction(Fighter::WALK);
					break;

				case SDLK_RIGHT:
					theFighter->players[0]->theCharacter->addToPosition(20, 0);
					theFighter->players[0]->theCharacter->setAction(Fighter::WALK);
					break;

				case SDLK_ESCAPE:
					theFighter->theGame->resetGame();
					theFighter->removePlayer();
					theFighter->removePlayer();
					theFighter->showMenu();
					break;

				default:
					break;
			}
			break;

		default:
			break;
	}
}









void Fighter::startGame()
{
	showMenu();
	theFighter->theGame->playMusic();
	theFighter->theGame->gameLoop();
}




void Fighter::showMenu()
{
	if( selectSound != NULL )
	{
		Mix_FreeChunk(selectSound);
		SDL_FreeSurface(selection);
	}
	
	theGame->setBackgroundImage("intro.png");
	theGame->setBackgroundMusic("menuBgm.wav");


	selection = IMG_Load("GameData\\selection.png");
	selectionPos.x = 240;
	selectionPos.y = 165;
	selectionPtr = theGame->registerObject(selection, selectionPos);

	selectSound = Mix_LoadWAV("GameData\\Static.wav");

	option = 0;
	theGame->setEventFunction(menuEventFunction);
}



void Fighter::showCharacterSelect()
{
	if( selectSound != NULL )
	{
		Mix_FreeChunk(selectSound);
		SDL_FreeSurface(selection);
	}

	theGame->setBackgroundImage("charBg.png");
	theGame->setBackgroundMusic("menuBgm.wav");

	for(int i=0; i<availablePlayers; i++)
		charPtrs[i] = theGame->registerObject(charImages[i], charPositions[i]);

	selection = IMG_Load("GameData\\charSelect.png");
	selectionPos.x = 92;
	selectionPos.y = 12;
	selectionPtr = theGame->registerObject(selection, selectionPos);

	selectSound = Mix_LoadWAV("GameData\\Static.wav");
	theGame->setEventFunction(charSelectEventFunction);
}


void Fighter::startFight()
{
	if( selectSound != NULL )
	{
		Mix_FreeChunk(selectSound);
		SDL_FreeSurface(selection);
	}
	theGame->setBackgroundImage("bg.bmp");

	addPlayers(currentPlayers[0]);
	addPlayers(currentPlayers[1]);
	players[0]->theCharacter->setPosition(80,350);
	players[0]->theCharacter->setAction(ENTRY);
	players[1]->theCharacter->setPosition(600,350);
	players[1]->theCharacter->setAction(ENTRY);
	players[1]->theCharacter->toggleFlip();

	theGame->setDisplayFunction(displayFunction);
	theGame->setCollisionFunction(collisionFunction);
	theGame->setIdleFunction(idleFunction);

	theGame->drawString(players[0]->theCharacter->getName(), 70, 10, 255, 255, 255, 18);
	theGame->drawString(players[1]->theCharacter->getName(), 470, 10, 255, 255, 255, 18);

	theGame->setEventFunction(fightEventFunction);
	theGame->setBackgroundMusic("bgm.wav");
}