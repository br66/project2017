/* The game loop file */

#include "include.h"

/* entities */
entity_t *wall;
entity_t *player;
entity_t *background;

/* create main or else (error LNK2001: unresolved external symbol _SDL_main) */
int main(int argc, char *argv[])
{
	//check level function
	/* if level one, show these things */
	/* if level two, show these things */

	entity_t *enemy1;
	entity_t *enemy2;
	entity_t *enemy3;

	entity_t *boss;
	
	bool done = false;

	/* Starting time for the timer */
	Uint32 start = 0;

	/* Flag for if the timer is running or not */
	bool running = true;
	
	/* Start everything (SDL, open window, etc.), make sure done successfully */
	if ( init() == false)
	{
		return 1;
	}
	
	/* Load all the file, check if done successfully */
	if ( load_Files() == false)
	{
		return 1;
	}
	
	/* Might move this */
	start = SDL_GetTicks();

	/* HUD */
	health.x = 10;
	health.y = 10;
	health.w = 100;
	health.h = 35;

	anger.x = 115;
	anger.y = 10;
	anger.w = 100;
	anger.h = 35;
	/* --- */

	/* PLAYER */
	player = Init_Ent();
	player->resetPosition = init_Position;
	player->handle_Input = handle_Input;
	player->move = move;
	player->show = show;

	init_Position(player);
	/* ------- */

	/* Enemy 1 */
	enemy1 = Init_Ent();
	
	enemy1->x = 600;
	enemy1->y = 350;

	//enemy1->bBox.x = enemy1->x;
	//enemy1->bBox.y = enemy1->y;

	enemy1->sprite = load_Image("sprite/red.png"); //sprites will later be loaded in sprite.c // declared in sprite.h

	enemy1->think = alphaThink;
	enemy1->nextThink = currentTime + 5000;

	//enemy1->bBox.w = 64;
	//enemy1->bBox.h = 64;
	
	enemy1->show = show_Enemy;
	/* ------- */

	/* Enemy 2 */
	enemy2 = Init_Ent();

	enemy2->sprite = load_Image("sprite/bluepng.png");

	enemy2->x = 770;
	enemy2->y = 350;

	//enemy2->bBox.x = enemy2->x;
	//enemy2->bBox.y = enemy2->y;

	//enemy2->bBox.w = 64;
	//enemy2->bBox.h = 64;

	enemy2->show = show_Enemy;

	enemy2->think = betaThink;
	enemy2->nextThink = currentTime + 7000;
	/* ------- */

	/* Enemy 3 */
	enemy3 = Init_Ent();

	enemy3->sprite = load_Image("sprite/green.png");

	enemy3->x = 900;
	enemy3->y = 350;

	//enemy3->bBox.x = enemy3->x;
	//enemy3->bBox.y = enemy3->y;

	//enemy3->bBox.w = 64;
	//enemy3->bBox.h = 64;
	enemy3->show = show_Enemy;

	enemy3->think = gammaThink;
	enemy3->nextThink = currentTime + 1000;
	/* ------- */

	/*  Boss  */
	boss = Init_Ent();

	boss->sprite = bossSprite;

	boss->x = 1000;
	boss->y = 300;

	//boss->bBox.x = boss->x;
	//boss->bBox.y = boss->y;

	boss->show = show_Enemy;

	boss->think = bossThink;
	boss->nextThink = currentTime + 9000;

	/* ------- */

	/* Wall */
	wall = Init_Ent();

	wall->sprite = load_Image("sprite/wall.jpg");

	wall->x = 400;
	wall->y = 40;

	wall->bBox.w = 40;
	wall->bBox.h = 400;

	wall->bBox.x = (int)wall->x; //?
	wall->bBox.y = (int)wall->y;
	/* ------- */

	level = 1; // when the game starts, we will be at level 1

	/* GAME ------------------------------------------------------------------------- */
	do
	{
		CheckLevel();
		//show_Surface (0, 0, bgSprite, screen, &camera);
		EntityAlive();
		EntityShow();

		while (SDL_PollEvent (&event))
		{
			handle_Input(player);

			if ( event.type == SDL_KEYDOWN )
			{
				switch ( event.key.keysym.sym )
				{
					case SDLK_UP: 
						if (Mix_PlayChannel (-1, scratch, 0) == -1) //arg. 1, -1 means looking for the next sound channel available
						{											//arg. 2, sound that will be played
							return 1;								//arg. 3, how many times sound will loop
						}
						break;
					case SDLK_1:
						level = 1;
						break;
					case SDLK_2:
						level = 2;
						break;
					case SDLK_RIGHT:
						if (Mix_PlayChannel (-1, low, 0) == -1)
						{
							return 1;
						}
						break;
					case SDLK_0:
						Mix_HaltMusic(); break;
					case SDLK_s:
						if (running == true)
						{
							running = false;
							start = 0;
						}
						else
						{
							running = true;
							start = SDL_GetTicks();
						}
					}
				}

			//If the user presses Quit (the x button on the window)
			if(event.type == SDL_QUIT)
			{
				//Game is done
				done = true;
			}

			move(player);
		}

		set_Camera(player);

		if (running == true)
			{
				char msg[20];
				sprintf( msg, "%s", FormatTimeString(start));

				seconds = TTF_RenderText_Solid (font, msg, textColor);
				show_Surface ((SCREEN_WIDTH - (float)seconds->w ) / 2, 50, seconds, screen, NULL);

				SDL_FreeSurface( seconds );
			}

		show(player);

		show_Enemy(wall);

		SDL_FillRect ( screen, &health, SDL_MapRGB ( screen->format, 0, 0xFF, 0 ) );
		SDL_FillRect ( screen, &anger, SDL_MapRGB ( screen->format, 0x77, 0x77, 0x77 ) );

		/* this gives us an array of all the possible keystates and whether a key is pressed or not */
		keystates = SDL_GetKeyState( NULL );

		delta = SDL_GetTicks() - currentTime;
		/* Constantly getting the raw time from SDL */
		currentTime = SDL_GetTicks();


		/* Function so that the screen is constantly updated so you can see things happening as they happen */
		SDL_Flip(screen);
	}
	
	while(!done);

	clear();

	exit(0);
	return 0;

	/* GAME -------------------------------------------------------------------------- */
}