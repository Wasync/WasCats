# include "../includes/wanted.h"

void make_circle(entity *entities, int count, SDL_Rect src)
{
	int orientation;
	int move_index;
	int gap;
	int i;
	int dx;
	int dy;
	int radius;

	movement_fn moves[2] =
	{
		move_circle,
		move_none,
	};

	if (!count)
		return;
	
	orientation = rand() % 2;
	move_index = rand() % 2;
	gap = 360 / 4;
	i = 0;
	dx = WINDOW_W / ((rand() % 3) + 1);
	dy = WINDOW_H / ((rand() % 3) + 1);
	radius = (rand() % 100) + 50;
	while (i < count)
	{
		entities[i].move = moves[move_index];
		entities[i].angle = i * gap;
		entities[i].dx = rand() % (WINDOW_W / 2) + 100;
		entities[i].dy = rand() % (WINDOW_W / 2) + 100;
		entities[i].r = radius;
		entities[i].sprite._dst.x = SDL_sin((i * gap * M_PI) / 180) * radius + entities[i].dx;
		entities[i].sprite._dst.y = SDL_cos((i * gap * M_PI) / 180) * radius + entities[i].dy;
		entities[i].sprite._src = src;
		++i;
	}
}

void make_line(entity *entities, int count, SDL_Rect src)
{
	int orientation;
	int move_index;
	int gap_x;
	int gap_y;
	int i;
	int x;
	int y;
	int dx;
	int dy;
	int gap;

	movement_fn moves[3] =
	{
		move_rand,
		move_sine,
		move_step
	};
	if (!count)
		return;

	orientation = rand() % 2;
	move_index = rand() % 3;

	gap_x = (WINDOW_W / count) * orientation;
	gap_y = (WINDOW_H / count) * (orientation ^ 1);
	// Gap should be at least the width and a half of a sprite
	if (gap_x <= SPRITE_W + SPRITE_W / 2)
		gap_x = SPRITE_W + SPRITE_W / 2;
	if (gap_y <= SPRITE_H + SPRITE_H / 2)
		gap_y = SPRITE_H + SPRITE_H / 2;
	
	// Sets gap x to window / count if oritention = 1 else 0
	// vice versa for gap y
	gap_x *= orientation;
	gap_y *= (orientation ^ 1);
	i = 0;
	x = rand() % (WINDOW_W - 50) + 50;
	y = rand() % (WINDOW_H - 50) + 50;

	// Dx inverse of dy (if dx = 0, dy = 1 etc)
	dx = rand() % 2;
	dy = dx ^ 1;
	while (i < count)
	{
		entities[i].sprite._dst.x = x;
		entities[i].sprite._dst.y = y;
		entities[i].move = moves[move_index];
		entities[i].angle = i;
		entities[i].dx = dx;
		entities[i].dy = dy;
		entities[i].sprite._src = src;
		x += gap_x;
		y += gap_y;
		++i;
	}
}

void make_rand(entity *entities, int count, SDL_Rect src)
{
	int i;
	int move_index;

	movement_fn moves[2] =
	{
		move_rand,
		move_none
	};

	if (!count)
		return;
	move_index = rand() % 2;
	i = 0;

	while (i < count)
	{
		entities[i].sprite._dst.x = (rand() % (WINDOW_W - 30)) + 30;
		entities[i].sprite._dst.y = (rand() % (WINDOW_H - 30)) + 30;
		if (entities[i].sprite._dst.x > WINDOW_W - 20)
			entities[i].sprite._dst.x -= WINDOW_W;
		if (entities[i].sprite._dst.x < 0)
			entities[i].sprite._dst.x *= -1;
		if (entities[i].sprite._dst.y > WINDOW_W - 20)
			entities[i].sprite._dst.y -= WINDOW_W;
		if (entities[i].sprite._dst.y < 0)
			entities[i].sprite._dst.y *= -1;
		entities[i].move = moves[move_index];
		entities[i].angle = i;
		// 				 	Direction 	  * 	Speed
		entities[i].dx = SIGN[rand() % 2] * ((rand() % 2) + 1);
		entities[i].dy = SIGN[rand() % 2] * ((rand() % 2) + 1);
		entities[i].sprite._src = src;
		++i;
	}
}

typedef void(*init_pos)(entity *entities, int, SDL_Rect);


void place_wanted(master *game_master)
{
	int random_num;

	random_num = rand() % game_master->sprite_count;

	game_master->wanted.sprite._src = (SDL_Rect){SPRITE_W * game_master->currentTex, 0, SPRITE_W, SPRITE_H};
	game_master->wanted.sprite._dst = game_master->entities[random_num].sprite._dst;
	game_master->wanted.move = game_master->entities[random_num].move;
	game_master->wanted.angle = game_master->entities[random_num].angle;
	game_master->wanted.dx = game_master->entities[random_num].dx;
	game_master->wanted.dy = game_master->entities[random_num].dy;
	//										Direction       *	    offset size 	*		axis
	game_master->wanted.sprite.dst->x += (SIGN[rand() % 2]) * (((rand() % 100) + 50) * game_master->wanted.dy);
	game_master->wanted.sprite.dst->y += (SIGN[rand() % 2]) * (((rand() % 100) + 50) * game_master->wanted.dx);
	wrap_position(&game_master->wanted);
	game_master->wanted.r = game_master->entities[random_num].r;
	game_master->wanted.r += (SIGN[rand() % 2])  * 20;
	
}


void game_next_level(master *game_master)
{
	int i;
	long random_num;

	SDLX_Time time;
	SDL_Rect src;
	init_pos inits[3] = {
		make_circle,
		make_line,
		make_rand
	};
	
	i = 0;
	time = SDLX_Time_Get();

	srand(SDL_GetTicks());
	game_master->level++;
	game_master->sprite_count = MIN(MIN_SPRITE * game_master->level, MAX_SPRITE - 1);
	src.h = SPRITE_H;
	src.w = SPRITE_W;
	src.y = 0;
	while(i < game_master->sprite_count)
	{
		// 	Theoretically divides the available amount of sprites randomly based on level up to max / min
		random_num = rand() % (MIN(MIN_SPRITE * game_master->level,(MAX_SPRITE - 1) / MIN_SPRITE)) + 1;
		src.x = rand() % TEXTURE_COUNT;
		// If the texture is the same as the chosen one for the wanted cat, choose another random one
		while (src.x == game_master->currentTex)
			src.x = rand() % TEXTURE_COUNT;
		src.x *= SPRITE_W;
		inits[rand() % 3](&game_master->entities[i], random_num, src);
		i += random_num;
	}
	place_wanted(game_master);
}
