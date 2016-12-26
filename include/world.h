#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

#include <glm/glm.hpp>

#include "game.h"

class Entity;
class Player;

class World
{
public:
	World(SDL_Renderer* renderer = nullptr, const glm::vec2& bounds = glm::vec2());
	~World();
	
	void handle(EVENT event, float dt);
	void update(float dt);
	void draw() const;
	
	void add(Entity* entity);
	
	const glm::vec2& bounds() const;
	SDL_Renderer* renderer() const;
private:
	SDL_Renderer* _renderer;
	std::vector<Entity*> _entities;
	glm::vec2 _bounds;
};
