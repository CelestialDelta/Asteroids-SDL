#include "../include/world.h"
#include "../include/entity.h"

World::World(SDL_Renderer* renderer, const glm::vec2& bounds)
	: _renderer(renderer), _entities(), _bounds(bounds)
{
	if(_renderer && bounds != glm::vec2())
	{
		Player* player = new Player(this, glm::vec2(), 50.0f, glm::vec2(400, 400), glm::vec2(13, 15), 0, 1.0f, 10.0f);
		_entities.push_back(player);
		
		Asteroid* asteroid = new Asteroid(this, glm::vec2(40, 40), glm::vec2(200, 200), glm::vec2(80, 70), 23);
		_entities.push_back(asteroid);
	}
}

World::~World()
{
	while(!_entities.empty())
	{
		delete _entities.back();
		_entities.pop_back();
	}
}

void World::handle(EVENT event, float dt)
{
	for(size_t i = 0; i < _entities.size(); i++)
	{
		_entities.at(i)->handle(event, dt);
	}
}

void World::update(float dt)
{
	for(size_t i = 0; i < _entities.size(); i++)
	{
		_entities.at(i)->update(dt);
	}
}

void World::draw() const
{
	for(size_t i = 0; i < _entities.size(); i++)
	{
		_entities.at(i)->draw(_renderer);
	}
}

void World::add(Entity* entity)
{
	_entities.push_back(entity);
}

const glm::vec2& World::bounds() const
{
	return _bounds;
}

SDL_Renderer* World::renderer() const
{
	return _renderer;
}
