#include "../include/world.h"
#include "../include/entity.h"

World::World(SDL_Renderer* renderer, const glm::vec2& bounds)
	: _renderer(renderer), _entities(), _statemap(), _bounds(bounds), _frozen()
{
	if(_renderer && bounds != glm::vec2())
	{
		Player* player = new Player(this, glm::vec2(), 50.0f, glm::vec2(400, 400), glm::vec2(13, 15), 0, 1.0f, 10.0f);
		_entities.push_back(player);
		
		Model model("../res/text.txt");
		
		//std::cout << model.vertices().size() << std::endl;
		
		Asteroid* asteroid = new Asteroid(this, model, glm::vec2(40, 40), glm::vec2(200, 200), glm::vec2(80, 70), 23);
		_entities.push_back(asteroid);
	}
	
	std::map<ENTITY_ID, ENTITY_STATE_ID> run_map;
	run_map[ENTITY_ID::PLAYER] = ENTITY_STATE_ID::PLAYER_DEFAULT;
	
	std::map<ENTITY_ID, ENTITY_STATE_ID> deb_map;
	deb_map[ENTITY_ID::PLAYER] = ENTITY_STATE_ID::PLAYER_RIGID;
	
	_statemap[GAMESTATE_ID::RUNNING] = run_map;
	_statemap[GAMESTATE_ID::DEBUG] = deb_map;
}

World::~World()
{
	while(!_entities.empty())
	{
		delete _entities.back();
		_entities.pop_back();
	}
}

void World::change_state(GAMESTATE_ID id)
{
	for(size_t i = 0; i < _entities.size(); i++)
	{
		Entity* entity = _entities.at(i);
		ENTITY_STATE_ID state = _statemap[id][entity->id()];
		if(entity->id() == ENTITY_ID::PLAYER)
		{
			//make Entity::push_state virtual function
			static_cast<Player*>(entity)->push_state(state);
		}
	}
}

void World::freeze()
{
	_frozen = !_frozen;
}

void World::handle(KEY_EVENT event, float dt)
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

const std::vector<Entity*>& World::entities() const
{
	return _entities;
}

bool World::frozen() const
{
	return _frozen;
}
