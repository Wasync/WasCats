# include "../includes/wanted.h"

void reset_formations(master *game_master)
{
	int i;

	i = 0;
	while (i < game_master->sprite_count)
	{
		game_master->entities[i].angle = 0;
		game_master->entities[i].move = NULL;
		game_master->entities[i].dx = 0;
		game_master->entities[i].dy = 0;
		++i;
	}
	game_master->sprite_count = 0;
}


master *game_init(void)
{
	master *game_master;
	
	SDL_Rect dst;
	SDL_Texture *tex;
	SDL_Texture *tex2;

	int 	i;

	game_master = SDL_calloc(1, sizeof(master));
	game_master->display = SDLX_Display_Get();
	game_master->entities = SDL_calloc(MAX_SPRITE + 1, sizeof(entity));
	game_master->timer = 15.0;
	game_master->level = 0;
	game_master->state = 0;
    game_master->currentTex = rand() % TEXTURE_COUNT;
	
	game_master->start = SDLX_Texture_Load("assets/start.png", game_master->display);
	game_master->transition = SDLX_Texture_Load("assets/transition.png", game_master->display);

    game_master->textures = SDLX_Texture_Load("assets/cats.png", game_master->display);
	i = 0;
	while (i < MAX_SPRITE)
	{
		SDLX_Sprite_Create(&(game_master->entities[i].sprite), 1, game_master->textures);
		dst.h = WINDOW_H / 9;
		dst.w = WINDOW_W / 9;
		dst.y = WINDOW_H / 2;
		dst.x = 0;
		game_master->entities[i].sprite._dst = dst;
		++i;
	}
	SDLX_Sprite_Create(&(game_master->wanted.sprite), 0, game_master->textures);
	game_master->wanted.sprite._dst = dst;

	reset_formations(game_master);
	return game_master;
}

void reset_game(master *game_master)
{
	game_master->timer = 15.0;
	game_master->level = 0;
	game_master->state = 0;
    game_master->score = 0;
    game_master->currentTex = rand() % TEXTURE_COUNT;
	reset_formations(game_master);
}

void cleanup_Game(master *game_master)
{
    int i;

    SDL_DestroyTexture(game_master->textures);
    SDL_DestroyTexture(game_master->start);
    SDL_DestroyTexture(game_master->transition);
    i = 0;
    while (i < MAX_SPRITE + 1)
    {
        SDL_free(&game_master->entities[i]);
        ++ i;
    }
    SDL_free(game_master);
}