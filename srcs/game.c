# include "../includes/wanted.h"


int collisions_loop(master *game_master)
{
	SDLX_Input input;
	int i;

	input = SDLX_Input_Get();
	i = 0;
	if (input.mouse_state == 2)
	{
		if (SDL_PointInRect(&input.mouse, game_master->wanted.sprite.dst))
		{
			// Goes to transition screen
			game_master->currentTex = rand() % TEXTURE_COUNT;
			game_master->score += game_master->timer * 10;
			game_master->state = 3;
			game_master->timer += 5;
			return 1;
		}
		while (i < game_master->sprite_count)
		{
			if (SDL_PointInRect(&input.mouse, game_master->entities[i].sprite.dst))
			{
				// This "removes" the sprite from the game. 
				// It won't move or render and is offset to prevent collisions
				game_master->entities[i].move = NULL;
				game_master->entities[i].sprite.dst->x = -100;
				game_master->entities[i].sprite.dst->y = -100;
				game_master->timer -= 15;
				game_master->score -= 10;
				return -1;
			}
			++i;
		}
	}
	return 0;
}

int game_loop(void *data)
{
	SDLX_Time	time;
	master		*game_master;
	char 		str[50];

	int i;
	int	collision;

	time = SDLX_Time_Get();
	game_master = data;

	i = 0;
	game_master->timer -= time.delta_time / 100.0;
	if (game_master->timer <= 0)
	{
		game_master->state = 2;
		return 1;
	}
	collision = collisions_loop(game_master);
	// SDL_Log("Timer %f", game_master->timer);

	if ( game_master->timer > 30)
		game_master->timer = 30;
	
	while (i < game_master->sprite_count)
	{
		if (game_master->entities[i].move)
		{
			game_master->entities[i].move(&game_master->entities[i],  time);
			SDLX_RenderQueue_Push(&game_master->entities[i].sprite);
		}
		++i;
	}

	game_master->wanted.move(&game_master->wanted,  time);

	SDLX_RenderQueue_Push(&game_master->wanted.sprite);
	SDL_snprintf(str, 50, "Level : %d", game_master->level);
	SDLX_RenderMessage_Aligned(game_master->display, SDLX_RIGHT_ALIGN, SDLX_TOP_ALIGN, DEFAULT_FONT_COLOR, 
	str);
	SDL_snprintf(str, 50, "%d", (int)game_master->timer);
	SDLX_RenderMessage_Aligned(game_master->display, SDLX_LEFT_ALIGN, SDLX_TOP_ALIGN, DEFAULT_FONT_COLOR, 
	str);
	return 1;
}

int end_loop(void *data)
{
	master		*game_master;
	SDLX_Input input;
	char 		str[50];

	game_master = data;
	input = SDLX_Input_Get();

	if (input.mouse_state == 2)
	{
		reset_game(game_master);
		game_master->state = 3;
		return 1;
	}

	SDLX_RenderMessage_Aligned(game_master->display, SDLX_CENTER_ALIGN, SDLX_TOP_ALIGN, DEFAULT_FONT_COLOR, 
	"GAME OVER");
	SDLX_RenderQueue_Push(&game_master->wanted.sprite);
	SDL_snprintf(str, 50, "Your score: %d \n Click to play again", (int)game_master->score);
	SDLX_RenderMessage_Aligned(game_master->display, SDLX_CENTER_ALIGN, SDLX_CENTER_ALIGN, DEFAULT_FONT_COLOR, 
	str);

	return 1;
}

int start_loop(void *data)
{
	master		*game_master;
 	SDLX_Input input;

	input = SDLX_Input_Get();
	game_master = data;

	if (input.mouse_state == 2)
	{
		reset_game(game_master);

		game_master->state = 3;
		return 1;
	}
	SDL_RenderCopy(game_master->display->renderer, game_master->start, NULL, NULL);

	return 1;
}

int transition_screen(void *data)
{
	static double	count = 0.0;
	SDLX_Time		time;
	master			*game_master;

	game_master = data;
	time = SDLX_Time_Get();

	SDL_RenderCopy(game_master->display->renderer, game_master->transition, NULL, NULL);
	SDL_RenderCopy(game_master->display->renderer,
				game_master->textures,
				&(SDL_Rect){SPRITE_W * game_master->currentTex, 0, SPRITE_W, SPRITE_H},
				&(SDL_Rect){WINDOW_W / 2 - SPRITE_W, WINDOW_H / 2 - SPRITE_H , SPRITE_W  * 2, SPRITE_H * 2}
				);
	
	if (count >= 1)
	{
		count = 0.0;
		game_next_level(game_master);
		game_master->state = 1;
	}
	count += time.delta_time / 100.0;
	return 1;
}