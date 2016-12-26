#include "../include/entity.h"
#include "../include/game.h"

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

void Entity::set_position(const glm::vec2& pos)
{
	_position = pos;
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

void Projectile::handle(EVENT event, float dt)
{
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

//=================================================================================================

Player::Player()
	: Entity(), _world(), _projectiles(), _delay(DEFAULT_PROJECTILE_DELAY), _vertices(), _rotationspeed()
{
}

Player::Player(World* world, const glm::vec2& vel, const float& max_vel, const glm::vec2& pos, const glm::vec2& size, const float& angle, const float& accel, const float& rspeed)
	: Entity(vel, pos, size, angle), _world(world), _projectiles(), _delay(DEFAULT_PROJECTILE_DELAY), _vertices(), _acceleration(accel), _maxvelocity(max_vel), _rotationspeed(rspeed)
{
	float x = pos.x;
	float y = pos.y;
	
	_vertices.push_back(glm::vec2(x, y - size.y)); //top vertex
	_vertices.push_back(glm::vec2(x - size.x, y + size.y)); //left vertex
	_vertices.push_back(glm::vec2(x + size.x, y + size.y)); //right vertex
}

Player::~Player()
{
	while(!_projectiles.empty())
	{
		delete _projectiles.back();
		_projectiles.pop_back();
	}
}

void Player::move(MOTION motion, float dt)
{
	switch(motion)
	{
	case MOTION::ACCELERATE:
	{
		float ay = _acceleration * cos(_angle) * dt;
		float ax = _acceleration * sin(_angle) * dt;
		if((std::abs(_velocity.y ) + ay) * 1000 >= _maxvelocity)
		{
			_velocity.y = _maxvelocity;
		}
		else
		{
			_velocity.y += ay;
		}
		
		if((std::abs(_velocity.x) + ax) * 1000 >= _maxvelocity)
		{
			_velocity.x = _maxvelocity;
		}
		else
		{
			_velocity.x += ax;
		}
	}
	break;
	case MOTION::ROTATE_RIGHT:
		_angle += math::to_radians(_rotationspeed);
		break;
	case MOTION::ROTATE_LEFT:
		_angle -= math::to_radians(_rotationspeed);
		break;
	}
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

void Player::handle(EVENT event, float dt)
{
	switch(event)
	{
	case EVENT::PLAYER_MOVE_ACCELERATE:
		move(MOTION::ACCELERATE, dt);
		break;
	case EVENT::PLAYER_MOVE_ROTATE_RIGHT:
		move(MOTION::ROTATE_RIGHT, dt);
		break;
	case EVENT::PLAYER_MOVE_ROTATE_LEFT:
		move(MOTION::ROTATE_LEFT, dt);
		break;
	case EVENT::PLAYER_SHOOT:
		shoot();
		break;
	case EVENT::PLAYER_DELETE_PROJECTILE:
		if(!_projectiles.empty())
		{
			delete _projectiles.front();
			_projectiles.erase(_projectiles.begin());
		}
		break;
	}
	
	std::cout << "Player Handle!" << std::endl;
	
	for(size_t i = 0; i < _projectiles.size(); i++)
	{
		//std::cout << "Handle!" << std::endl;
		_projectiles.at(i)->handle(event, dt);
	}
}

void Player::update(float dt)
{
	_position.x += _velocity.x;
	_position.y -= _velocity.y;
	
	float x = _position.x;
	float y = _position.y;
	
	float xbound = _world->bounds().x;
	float ybound = _world->bounds().y;
	
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
	
	_vertices.at(0) = glm::vec2(x, y - _size.y);
	_vertices.at(1) = glm::vec2(x - _size.x, y + _size.y);
	_vertices.at(2) = glm::vec2(x + _size.x, y + _size.y);
	
	for(size_t i = 0; i < _vertices.size(); i++)
	{
		_vertices.at(i) = math::rotate(_position, _vertices.at(i), _angle);
	}
	
	for(size_t i = 0; i < _projectiles.size(); i++)
	{
		_projectiles.at(i)->update(dt);
	}
	
	_delay.tick();
}

void Player::draw(SDL_Renderer* renderer) const
{
	glm::vec2 v1 = _vertices.at(0);
	glm::vec2 v2 = _vertices.at(1);
	glm::vec2 v3 = _vertices.at(2);
	
	trigonRGBA(renderer, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, 255, 255, 255, 255);
	
	for(size_t i = 0; i < _projectiles.size(); i++)
	{
		_projectiles.at(i)->draw(renderer);
	}
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

//=================================================================================================

Asteroid::Asteroid()
	: Entity(), _world(), _vertices()
{
}

Asteroid::Asteroid(World* world, const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle)
	: Entity(vel, pos, size, angle), _world(world), _vertices()
{
	_vertices.push_back(glm::vec2(0, 20));
	_vertices.push_back(glm::vec2(30, 0));
	_vertices.push_back(glm::vec2(50, 10));
	_vertices.push_back(glm::vec2(80, 0));
	_vertices.push_back(glm::vec2(80, 30));
	_vertices.push_back(glm::vec2(50, 70));
	_vertices.push_back(glm::vec2(20, 70));
	_vertices.push_back(glm::vec2(20, 40));
}

Asteroid::~Asteroid()
{
}

void Asteroid::handle(EVENT event, float dt)
{
}

void Asteroid::update(float dt)
{
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

void Asteroid::draw(SDL_Renderer* renderer) const
{
	for(size_t i = 0; i < _vertices.size(); i++)
	{
		glm::vec2 p1 = _vertices.at(i) + _position;
		glm::vec2 p2 = ((i != _vertices.size() - 1) ? _vertices.at(i + 1) : _vertices.at(0)) + _position;
		
		lineRGBA(renderer, p1.x, p1.y, p2.x, p2.y, 255, 255, 255, 255);
	}
	//circleRGBA(renderer, _position.x, _position.y, _size.x, 255, 255, 255, 255);
}

