#pragma once

#include <SDL2/SDL.h>

#include <iostream>
#include <stdlib.h>
#include <stdint.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

class World;

enum EVENT
{
	PLAYER_MOVE_ACCELERATE,
	PLAYER_MOVE_ROTATE_RIGHT,
	PLAYER_MOVE_ROTATE_LEFT,
	PLAYER_SHOOT,
	PLAYER_DELETE_PROJECTILE,
	COLLISION
};

class Game
{
public:
	Game();
	~Game();
	
	bool init();
	
	void handle(float dt);
	void update(float dt);
	void draw();

	bool is_running() const;
private:
	World* _world;

	SDL_Window* _window;
	SDL_Renderer* _renderer;
	bool _running;
};
