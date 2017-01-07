#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <iostream>

#include <glm/glm.hpp>

#include "game.h"

class Entity;
class Player;

enum class ENTITY_ID;
enum class ENTITY_STATE_ID;

class World
{
public:
	World(SDL_Renderer* renderer = nullptr, const glm::vec2& bounds = glm::vec2());
	~World();
	
	void change_state(GAMESTATE_ID id);
	void freeze();
	
	void handle(KEY_EVENT event, float dt);
	void update(float dt);
	void draw() const;
	
	void add(Entity* entity);
	
	const glm::vec2& bounds() const;
	SDL_Renderer* renderer() const;
	const std::vector<Entity*>& entities() const;
	
	bool frozen() const;
private:
	bool _frozen;
	SDL_Renderer* _renderer;
	std::vector<Entity*> _entities;
	std::map<GAMESTATE_ID, std::map<ENTITY_ID, ENTITY_STATE_ID>> _statemap;
	glm::vec2 _bounds;
};
