#include "../include/game.h"
#include "../include/world.h"

Game::Game()
	: _world(), _window(), _renderer(), _running()
{
}

Game::~Game()
{
	if(_window)
	{
		SDL_DestroyWindow(_window);
	}
	if(_renderer)
	{
		SDL_DestroyRenderer(_renderer);
	}
}

bool Game::init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to initialize SDL." << std::endl;
		return false;
	}
	
	_window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, NULL);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	
	if(_window && _renderer)
	{
		_world = new World(_renderer, glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
		_running = true;
		return true;
	}
}

void Game::handle(float dt)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			_running = false;
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym)
			{
			case SDLK_w:
				_world->handle(EVENT::PLAYER_MOVE_ACCELERATE, dt);
				break;
			case SDLK_d:
				_world->handle(EVENT::PLAYER_MOVE_ROTATE_RIGHT, dt);
				break;
			case SDLK_a:
				_world->handle(EVENT::PLAYER_MOVE_ROTATE_LEFT, dt);
				break;
			case SDLK_SPACE:
				_world->handle(EVENT::PLAYER_SHOOT, dt);
				break;
			}
		}
	}
}

void Game::update(float dt)
{
	_world->update(dt);
}

void Game::draw()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
	SDL_RenderClear(_renderer);

	_world->draw();

	SDL_RenderPresent(_renderer);
}

bool Game::is_running() const
{
	return _running;
}
