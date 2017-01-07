#pragma once

#include <SDL2/SDL.h>

#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include <bitset>
#include <string>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

class World;
class Game;

//=================================================================================================

enum class GAMESTATE_ID
{
	RUNNING,
	DEBUG
};

enum class KEY_EVENT
{
	NONE,
	PLAYER_MOVE_ACCELERATE,
	PLAYER_MOVE_ROTATE_RIGHT,
	PLAYER_MOVE_ROTATE_LEFT,
	PLAYER_MOVE_FORWARD,
	PLAYER_MOVE_BACKWARD,
	PLAYER_MOVE_RIGHT,
	PLAYER_MOVE_LEFT,
	PLAYER_SHOOT,
	POP_STATE,
	CHANGE_STATE_DEBUG,
	DEBUG_STATE_FREEZE,
	DEBUG_STATE_RIGID,
	GAME_EVENT_LISTEN
};

std::string print_key_event(KEY_EVENT event);

struct GameState
{
	GameState(Game* game);
	
	virtual KEY_EVENT handle_key(char key) = 0;
	virtual void handle(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
	virtual GAMESTATE_ID id() const = 0;
	
	Game* game;
};

struct GameStateRunning
	: public GameState
{
	GameStateRunning(Game* game);
	
	virtual KEY_EVENT handle_key(char key) override;
	virtual void handle(float dt) override;
	virtual void update(float dt) override;
	virtual void draw() override;
	
	virtual GAMESTATE_ID id() const override;
};

struct GameStateDebug
	: public GameState
{
	GameStateDebug(Game* game);
	
	virtual KEY_EVENT handle_key(char key) override;
	virtual void handle(float dt) override;
	virtual void update(float dt) override;
	virtual void draw() override;
	
	virtual GAMESTATE_ID id() const override;
	
	bool freeze; //freeze environment (NOT PLAYER)
	bool rigid; //rigid player movement
};

//=================================================================================================

#define ARG_BUFFER 50
#define BAD_ARG -1

#define ARG_DEBUG 0
#define ARG_FREEZE 1
#define ARG_RIGID 2

int32_t parse_arg(const std::string& arg);

class Game
{
public:
	Game();
	~Game();
	
	bool init(const std::bitset<ARG_BUFFER>& args);
	void stop();
	
	void listen();
	void ignore();
	
	void push_state(GAMESTATE_ID id);
	void pop_state();
	GAMESTATE_ID state() const;
	
	void handle(float dt);
	void update(float dt);
	void draw();

	bool is_running() const;
	bool is_listening() const;
	SDL_Renderer* renderer() const;
	World* world() const;
private:
	World* _world;

	std::vector<GameState*> _states;
	bool _listen; //print events

	SDL_Window* _window;
	SDL_Renderer* _renderer;
	bool _running;
};
