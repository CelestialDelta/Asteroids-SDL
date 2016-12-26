#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "math.h"
#include "world.h"
#include "game.h"
#include "clock.h"

class Entity
{
public:
	Entity();
	Entity(const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle);
	virtual ~Entity();
	
	virtual void handle(EVENT event, float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(SDL_Renderer* renderer) const = 0;
	
	void set_velocity(const glm::vec2& vel);
	
	void set_position(const glm::vec2& pos);
	void set_size(const glm::vec2& size);
	void set_angle(const float& angle);
	
	
	const glm::vec2& velocity() const;
	
	const glm::vec2& position() const;
	const glm::vec2& size() const;
	const float& angle() const;
protected:
	glm::vec2 _velocity;
	
	glm::vec2 _position;
	glm::vec2 _size;
	float _angle;
};

//=================================================================================================

#define DEFAULT_PROJECTILE_DURATION 2.0f

class Player;

class Projectile
	: public Entity
{
public:
	Projectile();
	Projectile(Player* player, const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle);
	virtual ~Projectile() override;
	
	virtual void handle(EVENT event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(SDL_Renderer* renderer) const override;
private:
	Timer _duration;
	Player* _player;
};

//=================================================================================================

#define DEFAULT_PROJECTILE_DELAY 0.50f

class Player
	: public Entity
{
public:
	enum MOTION
	{
		ACCELERATE,
		ROTATE_RIGHT,
		ROTATE_LEFT
	};
	Player();
	Player(World* world, const glm::vec2& vel, const float& max_vel, const glm::vec2& pos, const glm::vec2& size, const float& angle, const float& accel, const float& rspeed);
	virtual ~Player() override;
	
	void move(MOTION motion, float dt);
	void pop_projectile();
	void shoot();
	
	virtual void handle(EVENT event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(SDL_Renderer* renderer) const override;
	
	void set_acceleration(const float& accel);
	void set_max_velocity(const float& max_vel);
	void set_rotation_speed(const float& rspeed);
	
	const float& acceleration() const;
	const float& max_velocity() const;
	const float& rotation_speed() const;
	World* world() const;
private:
	World* _world;

	std::vector<Projectile*> _projectiles;
	Timer _delay;
	
	std::vector<glm::vec2> _vertices;
	float _acceleration;
	float _maxvelocity;
	float _rotationspeed;
};

//=================================================================================================

class Asteroid
	: public Entity
{
public:
	Asteroid();
	Asteroid(World* world, const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle);
	virtual ~Asteroid() override;
	
	virtual void handle(EVENT event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(SDL_Renderer* renderer) const override;
private:
	World* _world;
	
	std::vector<glm::vec2> _vertices;
};


