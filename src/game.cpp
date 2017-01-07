#include "../include/game.h"
#include "../include/world.h"

std::string print_key_event(KEY_EVENT event)
{
	//std::string 
	switch(event)
	{
	case KEY_EVENT::PLAYER_MOVE_ACCELERATE:
		return "PLAYER_MOVE_ACCELERATE";
	case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
		return "PLAYER_MOVE_ROTATE_RIGHT";
	case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
		return "PLAYER_MOVE_ROTATE_LEFT";
	case KEY_EVENT::PLAYER_MOVE_FORWARD:
		return "PLAYER_MOVE_FORWARD";
	case KEY_EVENT::PLAYER_MOVE_BACKWARD:
		return "PLAYER_MOVE_BACKWARD";
	case KEY_EVENT::PLAYER_MOVE_RIGHT:
		return "PLAYER_MOVE_RIGHT";
	case KEY_EVENT::PLAYER_MOVE_LEFT:
		return "PLAYER_MOVE_LEFT";
	case KEY_EVENT::PLAYER_SHOOT:
		return "PLAYER_SHOOT";
	case KEY_EVENT::POP_STATE:
		return "POP_STATE";
	case KEY_EVENT::CHANGE_STATE_DEBUG:
		return "CHANGE_STATE_DEBUG";
	case KEY_EVENT::DEBUG_STATE_FREEZE:
		return "DEBUG_STATE_FREEZE";
	case KEY_EVENT::DEBUG_STATE_RIGID:
		return "DEBUG_STATE_RIGID";
	case KEY_EVENT::GAME_EVENT_LISTEN:
		return "GAME_EVENT_LISTEN";;
	}
	return "NONE";
}

//=================================================================================================

GameState::GameState(Game* game)
	: game(game)
{
}

GameStateRunning::GameStateRunning(Game* game)
	: GameState(game)
{
}

KEY_EVENT GameStateRunning::handle_key(char key)
{
	switch(key)
	{
	case SDLK_w:
		return KEY_EVENT::PLAYER_MOVE_ACCELERATE;
	case SDLK_d:
		return KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT;
	case SDLK_a:
		return KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT;
	case SDLK_SPACE:
		return KEY_EVENT::PLAYER_SHOOT;
	case SDLK_q:
		return KEY_EVENT::CHANGE_STATE_DEBUG;
	case SDLK_l:
		return KEY_EVENT::GAME_EVENT_LISTEN;
	}
	return KEY_EVENT::NONE;
}

void GameStateRunning::handle(float dt)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			game->stop();
		case SDL_KEYDOWN:
		{
			KEY_EVENT kevent = handle_key(event.key.keysym.sym);
			if(game->is_listening())
			{
				std::cout << print_key_event(kevent) << std::endl;
			}
			switch(kevent)
			{
			case KEY_EVENT::PLAYER_MOVE_ACCELERATE:
			case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
			case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
			case KEY_EVENT::PLAYER_SHOOT:
				game->world()->handle(kevent, dt);
				break;
			case KEY_EVENT::CHANGE_STATE_DEBUG:
					game->push_state(GAMESTATE_ID::DEBUG);
				break;
			case KEY_EVENT::GAME_EVENT_LISTEN:
				if(game->is_listening())
				{
					game->ignore();
				}
				else
				{
					game->listen();
				}
				break;
			}
		}
		}
	}	
}

void GameStateRunning::update(float dt)
{
	game->world()->update(dt);
}

void GameStateRunning::draw()
{
	SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 0);
	SDL_RenderClear(game->renderer());

	game->world()->draw();

	SDL_RenderPresent(game->renderer());
}

GAMESTATE_ID GameStateRunning::id() const
{
	return GAMESTATE_ID::RUNNING;
}

GameStateDebug::GameStateDebug(Game* game)
	: GameState(game), freeze(), rigid()
{
}

KEY_EVENT GameStateDebug::handle_key(char key)
{
	//state still requires this to be true; inverted for time being
	if(!rigid)
	{
		switch(key)
		{
		case SDLK_w:
			return KEY_EVENT::PLAYER_MOVE_FORWARD;
		case SDLK_d:
			return KEY_EVENT::PLAYER_MOVE_RIGHT;
		case SDLK_a:
			return KEY_EVENT::PLAYER_MOVE_LEFT;
		case SDLK_s:
			return KEY_EVENT::PLAYER_MOVE_BACKWARD;
		case SDLK_LEFTBRACKET:
			return KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT;
		case SDLK_RIGHTBRACKET:
			return KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT;
		}
	}

	switch(key)
	{
	case SDLK_w:
		return KEY_EVENT::PLAYER_MOVE_ACCELERATE;
	case SDLK_d:
		return KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT;
	case SDLK_a:
		return KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT;
	case SDLK_SPACE:
		return KEY_EVENT::PLAYER_SHOOT;
	case SDLK_q:
		return KEY_EVENT::POP_STATE;
	case SDLK_f:
		return KEY_EVENT::DEBUG_STATE_FREEZE;
	case SDLK_r:
		return KEY_EVENT::DEBUG_STATE_RIGID;
	case SDLK_l:
		return KEY_EVENT::GAME_EVENT_LISTEN;
	}
	return KEY_EVENT::NONE;
}

void GameStateDebug::handle(float dt)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			game->stop();
		case SDL_KEYDOWN:
		{
			KEY_EVENT kevent = handle_key(event.key.keysym.sym);
			if(game->is_listening())
			{
				std::cout << print_key_event(kevent) << std::endl;
			}
			switch(kevent)
			{
			case KEY_EVENT::PLAYER_MOVE_ACCELERATE:
			case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
			case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
			case KEY_EVENT::PLAYER_SHOOT:
			case KEY_EVENT::PLAYER_MOVE_FORWARD:
			case KEY_EVENT::PLAYER_MOVE_RIGHT:
			case KEY_EVENT::PLAYER_MOVE_LEFT:
			case KEY_EVENT::PLAYER_MOVE_BACKWARD:
				game->world()->handle(kevent, dt);
				break;
			case KEY_EVENT::POP_STATE:
				game->pop_state();
				break;
			case KEY_EVENT::DEBUG_STATE_FREEZE:
				freeze = !freeze;
				game->world()->freeze();
				break;
			case KEY_EVENT::DEBUG_STATE_RIGID:
				rigid = !rigid;
				break;
			case KEY_EVENT::GAME_EVENT_LISTEN:
				if(game->is_listening())
				{
					game->ignore();
				}
				else
				{
					game->listen();
				}
				break;
			}
		}
		}
	}
}

void GameStateDebug::update(float dt)
{
	game->world()->update(dt);
}

void GameStateDebug::draw()
{
	SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 0);
	SDL_RenderClear(game->renderer());

	game->world()->draw();

	SDL_RenderPresent(game->renderer());
}

GAMESTATE_ID GameStateDebug::id() const
{
	return GAMESTATE_ID::DEBUG;
}

//=================================================================================================

int32_t parse_arg(const std::string& arg)
{
	if(arg == "-debug")
	{
		return ARG_DEBUG;
	}
	else if(arg == "-rigid")
	{
		return ARG_RIGID;
	}
	else if(arg == "-freeze")
	{
		return ARG_FREEZE;
	}
	return BAD_ARG;
}

Game::Game()
	: _world(), _states(), _listen(), _window(), _renderer(), _running()
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

bool Game::init(const std::bitset<ARG_BUFFER>& args)
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
		//_states.push_back(new GameStateRunning(this));
		if(args[ARG_DEBUG])
		{
			_states.push_back(new GameStateDebug(this));
			if(args[ARG_FREEZE])
			{
				static_cast<GameStateDebug*>(_states.back())->freeze = true;
			}
			if(args[ARG_RIGID])
			{
				static_cast<GameStateDebug*>(_states.back())->rigid = true;
			}
		}
		else
		{
			_states.push_back(new GameStateRunning(this));
		}
		_running = true;
		return true;
	}
}

void Game::stop()
{
	_running = false;
}

void Game::listen()
{
	_listen = true;
}

void Game::ignore()
{
	_listen = false;
}

void Game::push_state(GAMESTATE_ID id)
{
	GameState* state = nullptr;
	switch(id)
	{
	case GAMESTATE_ID::RUNNING:
		state = new GameStateRunning(this);
		break;
	case GAMESTATE_ID::DEBUG:
		state = new GameStateDebug(this);
		break;
	}
	if(state)
	{
		_states.push_back(state);
		_world->change_state(id);
	}
}

void Game::pop_state()
{
	GameState* state = _states.back();
	_states.pop_back();
	delete state;
	
	_world->change_state(_states.back()->id());
}

GAMESTATE_ID Game::state() const
{
	return _states.back()->id();
}

void Game::handle(float dt)
{
	_states.back()->handle(dt);
}

void Game::update(float dt)
{
	_states.back()->update(dt);
}

void Game::draw()
{
	_states.back()->draw();
}

bool Game::is_running() const
{
	return _running;
}

bool Game::is_listening() const
{
	return _listen;
}


SDL_Renderer* Game::renderer() const
{
	return _renderer;
}

World* Game::world() const
{
	return _world;
}




