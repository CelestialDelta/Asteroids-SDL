#include "../include/entity.h"
#include "../include/game.h"

//=================================================================================================

Entity::Entity()
	: _velocity(), _position(), _size(), _angle()
{
}

Entity::Entity(const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle)
	: _velocity(vel), _position(pos), _size(size), _angle(angle)
{
}

Entity::~Entity()
{
}

void Entity::set_velocity(const glm::vec2& vel)
{
	_velocity = vel;
}

void Entity::set_x_velocity(const float& vx)
{
	_velocity.x = vx;
}

void Entity::set_y_velocity(const float& vy)
{
	_velocity.y = vy;
}

void Entity::set_position(const glm::vec2& pos)
{
	_position = pos;
}

void Entity::set_x_position(const float& x)
{
	_position.x = x;
}

void Entity::set_y_position(const float& y)
{
	_position.y = y;
}

void Entity::set_angle(const float& angle)
{
	_angle = angle;
}

const glm::vec2& Entity::velocity() const
{
	return _velocity;
}


const glm::vec2& Entity::position() const
{
	return _position;
}

const glm::vec2& Entity::size() const
{
	return _size;
}

const float& Entity::angle() const
{
	return _angle;
}

//=================================================================================================

Projectile::Projectile()
	: Entity(), _duration(DEFAULT_PROJECTILE_DURATION), _player()
{
}

Projectile::Projectile(Player* player, const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle)
	: Entity(vel, pos, size, angle), _duration(DEFAULT_PROJECTILE_DURATION), _player(player)
{
	_duration.start();
}

Projectile::~Projectile()
{
}

void Projectile::handle(KEY_EVENT event, float dt)
{
	std::vector<Entity*> ents = _player->world()->entities();
	for(size_t i = 0; i < ents.size(); i++)
	{
		Entity* ent = ents.at(i);
		if(ent->id() == ENTITY_ID::ASTEROID)
		{
			/*if(static_cast<Asteroid*>(ent)->collide(_position, _size))
			{
				std::cout << "Collision" << std::endl;
			}*/
		}
	}
}

void Projectile::update(float dt)
{
	_position.x += _velocity.x * sin(_angle) * dt;
	_position.y += _velocity.y * -cos(_angle) * dt;
	
	float x = _position.x;
	float y = _position.y;
	
	float xbound = _player->world()->bounds().x;
	float ybound = _player->world()->bounds().y;
	
	if(x + _size.y <= 0)
	{
		_position.x = xbound - _size.y;
	}
	if(x - _size.y >= xbound)
	{
		_position.x = 0 - _size.y;
	}
	
	if(y + _size.y <= 0)
	{
		_position.y = ybound + _size.y;
	}
	if(y - _size.y >= ybound)
	{
		_position.y = 0 - _size.y;
	}
	
	_duration.tick();
	if(_duration.time_left() == 0)
	{
		_player->pop_projectile();
	}
}

void Projectile::draw(SDL_Renderer* renderer) const
{
	circleRGBA(renderer, _position.x, _position.y, _size.x, 255, 255, 255, 255);
}

ENTITY_ID Projectile::id() const
{
	return ENTITY_ID::PROJECTILE;
}

//=================================================================================================

PlayerState::PlayerState(Player* player)
	: player(player)
{
}

void PlayerState::draw()
{
	std::vector<glm::vec2> vertices = player->vertices();
	glm::vec2 v1 = vertices.at(0);
	glm::vec2 v2 = vertices.at(1);
	glm::vec2 v3 = vertices.at(2);
	
	SDL_Renderer* renderer = player->world()->renderer();
	
	trigonRGBA(renderer, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, 255, 255, 255, 255);

	std::vector<Projectile*> projs = player->projectiles();	
	for(size_t i = 0; i < projs.size(); i++)
	{
		projs.at(i)->draw(renderer);
	}
}

PlayerStateDefault::PlayerStateDefault(Player* player)
	: PlayerState(player)
{
}

void PlayerStateDefault::move(KEY_EVENT motion, float dt)
{
	switch(motion)
	{
	case KEY_EVENT::PLAYER_MOVE_ACCELERATE:
	{
		float angle = player->angle();
		float accel = player->acceleration();
		
		float vy = player->velocity().y;
		float vx = player->velocity().x;
		
		float mv = player->max_velocity();
		
		float ay = accel * cos(angle) * dt;
		float ax = accel * sin(angle) * dt;
		if((std::abs(vy) + ay) * 1000 >= mv)
		{
			player->set_y_velocity(mv);
		}
		else
		{
			vy += ay;
			player->set_y_velocity(vy);
		}
		
		if((std::abs(vx) + ax) * 1000 >= mv)
		{
			player->set_x_velocity(mv);
		}
		else
		{
			vx += ax;
			player->set_x_velocity(vx);
		}
		break;
	}
	case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
	{
		float angle = player->angle() + math::to_radians(player->rotation_speed());
		player->set_angle(angle);
		break;
		
	}
	case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
	{
		float angle = player->angle() - math::to_radians(player->rotation_speed());
		player->set_angle(angle);
		break;
	}
	}
}

void PlayerStateDefault::handle(KEY_EVENT event, float dt)
{
	switch(event)
	{
	case KEY_EVENT::PLAYER_MOVE_ACCELERATE:
	case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
	case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
		move(event, dt);
		break;
	case KEY_EVENT::PLAYER_SHOOT:
		player->shoot();
		break;
	}
	
	std::vector<Projectile*> projs = player->projectiles();
	for(size_t i = 0; i < projs.size(); i++)
	{
		projs.at(i)->handle(event, dt);
	}
}

void PlayerStateDefault::update(float dt)
{
	float x = player->position().x;
	float y = player->position().y;
	
	float vx = player->velocity().x;
	float vy = player->velocity().y;
	
	float sx = player->size().x;
	float sy = player->size().y;
	
	glm::vec2 np(x + vx, y - vy);
	player->set_position(np);
	
	float xbound = player->world()->bounds().x;
	float ybound = player->world()->bounds().y;
	
	if(x + 2 * sx <= 0)
	{
		player->set_x_position(xbound + sx);
	}
	if(x - 2 * sx >= xbound)
	{
		player->set_x_position(0 - sx);
	}
	
	if(y + 2 * sy <= 0)
	{
		player->set_y_position(ybound + sy);
	}
	if(y - 2 * sy >= ybound)
	{
		player->set_y_position(0 - sy);
	}

	std::vector<glm::vec2> vertices = player->vertices();	
	std::vector<Projectile*> projs = player->projectiles();
	float angle = player->angle();
	
	vertices.at(0) = glm::vec2(x, y - sy);
	vertices.at(1) = glm::vec2(x - sx, y + sy);
	vertices.at(2) = glm::vec2(x + sx, y + sy);
	player->set_vertices(vertices);
	
	for(size_t i = 0; i < vertices.size(); i++)
	{
		vertices.at(i) = math::rotate(player->position(), vertices.at(i), angle);
	}
	player->set_vertices(vertices);
	
	for(size_t i = 0; i < projs.size(); i++)
	{
		projs.at(i)->update(dt);
	}
}

//=================================================================================================

PlayerStateRigid::PlayerStateRigid(Player* player, const float& jump)
	: PlayerState(player), jump(jump)
{
}

void PlayerStateRigid::move(KEY_EVENT motion, float dt)
{
	float x = player->position().x;
	float y = player->position().y;	
	
	switch(motion)
	{
	case KEY_EVENT::PLAYER_MOVE_FORWARD:
		player->set_y_position(y - jump * dt);
		break;
	case KEY_EVENT::PLAYER_MOVE_BACKWARD:
		player->set_y_position(y + jump * dt);
		break;
	case KEY_EVENT::PLAYER_MOVE_RIGHT:
		player->set_x_position(x + jump * dt);
		break;
	case KEY_EVENT::PLAYER_MOVE_LEFT:
		player->set_x_position(x - jump * dt);
		break;
	case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
	{
		float angle = player->angle() + math::to_radians(player->rotation_speed());
		player->set_angle(angle);
		break;
	}
	case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
	{
		float angle = player->angle() - math::to_radians(player->rotation_speed());
		player->set_angle(angle);
		break;
	}
	}
}

void PlayerStateRigid::handle(KEY_EVENT event, float dt)
{
	switch(event)
	{
	case KEY_EVENT::PLAYER_MOVE_FORWARD:
	case KEY_EVENT::PLAYER_MOVE_BACKWARD:
	case KEY_EVENT::PLAYER_MOVE_RIGHT:
	case KEY_EVENT::PLAYER_MOVE_LEFT:
	case KEY_EVENT::PLAYER_MOVE_ROTATE_LEFT:
	case KEY_EVENT::PLAYER_MOVE_ROTATE_RIGHT:
		move(event, dt);
		break;
	case KEY_EVENT::PLAYER_SHOOT:
		player->shoot();
		break;
	}
	
	std::vector<Projectile*> projs = player->projectiles();
	for(size_t i = 0; i < projs.size(); i++)
	{
		projs.at(i)->handle(event, dt);
	}
}

void PlayerStateRigid::update(float dt)
{
	float x = player->position().x;
	float y = player->position().y;
	
	float sx = player->size().x;
	float sy = player->size().y;
	
	float xbound = player->world()->bounds().x;
	float ybound = player->world()->bounds().y;
	
	if(x + 2 * sx <= 0)
	{
		player->set_x_position(xbound + sx);
	}
	if(x - 2 * sx >= xbound)
	{
		player->set_x_position(0 - sx);
	}
	
	if(y + 2 * sy <= 0)
	{
		player->set_y_position(ybound + sy);
	}
	if(y - 2 * sy >= ybound)
	{
		player->set_y_position(0 - sy);
	}

	std::vector<glm::vec2> vertices = player->vertices();	
	std::vector<Projectile*> projs = player->projectiles();
	float angle = player->angle();
	
	vertices.at(0) = glm::vec2(x, y - sy);
	vertices.at(1) = glm::vec2(x - sx, y + sy);
	vertices.at(2) = glm::vec2(x + sx, y + sy);
	player->set_vertices(vertices);
	
	for(size_t i = 0; i < vertices.size(); i++)
	{
		vertices.at(i) = math::rotate(player->position(), vertices.at(i), angle);
	}
	player->set_vertices(vertices);
	
	for(size_t i = 0; i < projs.size(); i++)
	{
		projs.at(i)->update(dt);
	}
}

//=================================================================================================

Player::Player()
	: Entity(), _world(), _projectiles(), _delay(DEFAULT_PROJECTILE_DELAY), _vertices(), _rotationspeed()
{
}

Player::Player(World* world, const glm::vec2& vel, const float& max_vel, const glm::vec2& pos, const glm::vec2& size, const float& angle, const float& accel, const float& rspeed)
	: Entity(vel, pos, size, angle), _world(world), _projectiles(), _delay(DEFAULT_PROJECTILE_DELAY), _states(), _vertices(), _acceleration(accel), _maxvelocity(max_vel), _rotationspeed(rspeed)
{
	float x = pos.x;
	float y = pos.y;
	
	_vertices.push_back(glm::vec2(x, y - size.y)); //top vertex
	_vertices.push_back(glm::vec2(x - size.x, y + size.y)); //left vertex
	_vertices.push_back(glm::vec2(x + size.x, y + size.y)); //right vertex
	
	_states.push_back(new PlayerStateDefault(this));
}

Player::~Player()
{
	while(!_projectiles.empty())
	{
		delete _projectiles.back();
		_projectiles.pop_back();
	}
}

void Player::push_state(ENTITY_STATE_ID id)
{
	switch(id)
	{
	case ENTITY_STATE_ID::PLAYER_DEFAULT:
		_states.push_back(new PlayerStateDefault(this));
		break;
	case ENTITY_STATE_ID::PLAYER_RIGID:
		_states.push_back(new PlayerStateRigid(this, PLAYER_DEBUG_MOTION));
		break;
	case ENTITY_STATE_ID::PLAYER_STATIONARY:
		//
		break;
	}
}

void Player::pop_state()
{
	PlayerState* state = _states.back();
	_states.pop_back();
	delete state;
}

void Player::pop_projectile()
{
	if(!_projectiles.empty())
	{
		delete _projectiles.front();
		_projectiles.erase(_projectiles.begin());
	}
}

void Player::shoot()
{
	if(_delay.time_left() == 0 || !_delay.is_ticking())
	{
		Projectile* proj = new Projectile(this, glm::vec2(1000, 1000), _vertices.at(0), glm::vec2(1, 1), _angle);
		_projectiles.push_back(proj);
		
		_delay.reset();
		_delay.start();
	}
}

void Player::handle(KEY_EVENT event, float dt)
{
	_states.back()->handle(event, dt);
}

void Player::update(float dt)
{	
	_states.back()->update(dt);
	_delay.tick();
}

void Player::draw(SDL_Renderer* renderer) const
{
	_states.back()->draw();
}

void Player::set_acceleration(const float& accel)
{
	_acceleration = accel;
}

void Player::set_max_velocity(const float& max_vel)
{
	_maxvelocity = max_vel;
}

void Player::set_rotation_speed(const float& rspeed)
{
	_rotationspeed = rspeed;
}

void Player::set_vertices(const std::vector<glm::vec2>& vertices)
{
	_vertices = vertices;
}

const float& Player::acceleration() const
{
	return _acceleration;
}

const float& Player::max_velocity() const
{
	return _maxvelocity;
}

const float& Player::rotation_speed() const
{
	return _rotationspeed;
}

World* Player::world() const
{
	return _world;
}

const std::vector<Projectile*>& Player::projectiles() const
{
	return _projectiles;
}

const std::vector<glm::vec2>& Player::vertices() const
{
	return _vertices;
}

ENTITY_ID Player::id() const
{
	return ENTITY_ID::PLAYER;
}

//=================================================================================================

Asteroid::Asteroid()
	: Entity(), _world(), _model()//, _vertices()
{
}

Asteroid::Asteroid(World* world, const Model& model, const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle)
	: Entity(vel, pos, size, angle), _model(model), _world(world)//, _vertices()
{
	/*_vertices.push_back(glm::vec2(0, 20));
	_vertices.push_back(glm::vec2(30, 0));
	_vertices.push_back(glm::vec2(50, 10));
	_vertices.push_back(glm::vec2(80, 0));
	_vertices.push_back(glm::vec2(80, 30));
	_vertices.push_back(glm::vec2(50, 70));
	_vertices.push_back(glm::vec2(20, 70));
	_vertices.push_back(glm::vec2(20, 40));*/
}

Asteroid::~Asteroid()
{
}

bool Asteroid::collide(const std::vector<glm::vec2>& vertices, const glm::vec2& position) const
{
	for(size_t i = 0; i < vertices.size(); i++)
	{	
		glm::vec2 p1 = vertices.at(i) + position;
		glm::vec2 p2 = ((i != vertices.size() - 1) ? vertices.at(i + 1) : vertices.at(0)) + position;
	}

	return false;
}

void Asteroid::handle(KEY_EVENT event, float dt)
{
}

void Asteroid::update(float dt)
{
	if(!_world->frozen())
	{
		//std::cout << (g_debug) << std::endl;
		_position.x += _velocity.x * sin(_angle) * dt;
		_position.y += _velocity.y * cos(_angle) * dt;
	
		float x = _position.x;
		float y = _position.y;
	
		float xbound = _world->bounds().x;
		float ybound = _world->bounds().y;
	
		//check graph paper for height
		if(x + _size.x <= 0)
		{
			_position.x = xbound + _size.x;
		}
		if(x - _size.x >= xbound)
		{
			_position.x = 0 - _size.x;
		}
	
		if(y + _size.y <= 0)
		{
			_position.y = ybound + _size.y;
		}
		if(y - _size.y >= ybound)
		{
			_position.y = 0 - _size.y;
		}
	}
}

void Asteroid::draw(SDL_Renderer* renderer) const
{
	std::vector<glm::vec2> vertices = _model.vertices();
	//std::cout << vertices.size() << std::endl;
	for(size_t i = 0; i < vertices.size(); i++)
	{
		glm::vec2 p1 = vertices.at(i) + _position;
		glm::vec2 p2 = ((i != vertices.size() - 1) ? vertices.at(i + 1) : vertices.at(0)) + _position;
		
		lineRGBA(renderer, p1.x, p1.y, p2.x, p2.y, 255, 255, 255, 255);
	}
	//circleRGBA(renderer, _position.x, _position.y, _size.x, 255, 255, 255, 255);
}

ENTITY_ID Asteroid::id() const
{
	return ENTITY_ID::ASTEROID;
}
