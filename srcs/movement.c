# include "../includes/wanted.h"

void wrap_position(entity *entity)
{

	if (entity->sprite.dst->x >= WINDOW_W - entity->sprite.dst->w / 5)
		entity->sprite.dst->x = 0;
	if (entity->sprite.dst->y >= WINDOW_H - entity->sprite.dst->h / 5)
		entity->sprite.dst->y = 0;
	if (entity->sprite.dst->x + entity->sprite.dst->w - entity->sprite.dst->w / 4 < 0)
		entity->sprite.dst->x = WINDOW_W - entity->sprite.dst->w;
	if (entity->sprite.dst->y + entity->sprite.dst->h - entity->sprite.dst->h / 4 < 0)
		entity->sprite.dst->y = WINDOW_H - entity->sprite.dst->h;
}

void move_circle(entity *self, SDLX_Time time)
{
	self->sprite.dst->x = round(SDL_sin(self->angle * M_PI / 180) * self->r) + self->dx;
	self->sprite.dst->y = round(SDL_cos(self->angle * M_PI / 180) * self->r) + self->dy;

	self->angle += 1 * time.delta_time;
}

void move_none(entity *self, SDLX_Time time)
{
	return ;
}

void move_sine(entity *self, SDLX_Time time)
{
	int i;

	i = 0;
	if (self->dx)
	{
		self->sprite.dst->y += round((SDL_sin(self->angle * 0.1) * 5));
		self->sprite.dst->x += 1;
	}
	else
	{
		self->sprite.dst->x += round((SDL_sin(self->angle * 0.1) * 5));
		self->sprite.dst->y += 1;
	}
	wrap_position(self);
	self->angle += 1 * time.delta_time;
	++i;
}

void move_rand(entity *self, SDLX_Time time)
{
	self->sprite.dst->x += self->dx;
	self->sprite.dst->y += self->dy;

	// Bounces sprite off the edge of the screen
	if (self->sprite.dst->x + self->sprite.dst->w > WINDOW_W || 
		self->sprite.dst->x < 0)
		self->dx *= -1;
	if (self->sprite.dst->y + self->sprite.dst->h > WINDOW_H || 
		self->sprite.dst->y < 0)
		self->dy *= -1;
}

void move_step(entity *self, SDLX_Time time)
{
	self->sprite.dst->x += self->dx;
	self->sprite.dst->y += self->dy;
	wrap_position(self);
}