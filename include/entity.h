#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "math.h"
#include "world.h"
#include "game.h"
#include "clock.h"
#include "model.h"

enum class ENTITY_ID
{
	PLAYER,
	PROJECTILE,
	ASTEROID
};

enum class ENTITY_STATE_ID
{
	PLAYER_DEFAULT,
	PLAYER_RIGID,
	PLAYER_STATIONARY
};

class Entity
{
public:
	Entity();
	Entity(const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle);
	virtual ~Entity();
	
	virtual void handle(KEY_EVENT event, float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(SDL_Renderer* renderer) const = 0;
	
	virtual ENTITY_ID id() const = 0;
	
	void set_velocity(const glm::vec2& vel);
	void set_x_velocity(const float& vx);
	void set_y_velocity(const float& vy);
	
	void set_position(const glm::vec2& pos);
	void set_x_position(const float& x);
	void set_y_position(const float& y);
	
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
	
	virtual void handle(KEY_EVENT event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(SDL_Renderer* renderer) const override;
	virtual ENTITY_ID id() const override;
private:
	Timer _duration;
	Player* _player;
};

//=================================================================================================

#define DEFAULT_PROJECTILE_DELAY 0.50f

#define PLAYER_DEBUG_MOTION 50000

struct PlayerState
{
	PlayerState(Player* player);
	
	virtual void move(KEY_EVENT motion, float dt) = 0;
	virtual void handle(KEY_EVENT event, float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw();
	
	Player* player;
};

struct PlayerStateDefault
	: public PlayerState
{
	PlayerStateDefault(Player* player);
	
	virtual void move(KEY_EVENT motion, float dt) override;
	virtual void handle(KEY_EVENT event, float dt) override;
	virtual void update(float dt) override;
};

struct PlayerStateRigid
	: public PlayerState
{
	PlayerStateRigid(Player* player, const float& jump);
	
	virtual void move(KEY_EVENT motion, float dt) override;
	virtual void handle(KEY_EVENT event, float dt) override;
	virtual void update(float dt) override;
	//virtual void draw(); //why?
	
	float jump;
};

//=================================================================================================

class Player
	: public Entity
{
public:
	Player();
	Player(World* world, const glm::vec2& vel, const float& max_vel, const glm::vec2& pos, const glm::vec2& size, const float& angle, const float& accel, const float& rspeed);
	virtual ~Player() override;
	
	void push_state(ENTITY_STATE_ID id);
	void pop_state();
	
	void pop_projectile();
	void shoot();
	
	virtual void handle(KEY_EVENT event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(SDL_Renderer* renderer) const override;
	virtual ENTITY_ID id() const override;
	
	void set_acceleration(const float& accel);
	void set_max_velocity(const float& max_vel);
	void set_rotation_speed(const float& rspeed);
	void set_vertices(const std::vector<glm::vec2>& vertices);
	
	const float& acceleration() const;
	const float& max_velocity() const;
	const float& rotation_speed() const;
	World* world() const;
	const std::vector<Projectile*>& projectiles() const;
	const std::vector<glm::vec2>& vertices() const;
private:
	World* _world;

	std::vector<Projectile*> _projectiles;
	Timer _delay;
	
	std::vector<PlayerState*> _states;
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
	Asteroid(World* world, const Model& model, const glm::vec2& vel, const glm::vec2& pos, const glm::vec2& size, const float& angle);
	virtual ~Asteroid() override;
	
	bool collide(const std::vector<glm::vec2>& vertices, const glm::vec2& position) const;
	
	virtual void handle(KEY_EVENT event, float dt) override;
	virtual void update(float dt) override;
	virtual void draw(SDL_Renderer* renderer) const override;
	virtual ENTITY_ID id() const override;
private:
	World* _world;
	Model _model;
	
	//std::vector<glm::vec2> _vertices;
};


