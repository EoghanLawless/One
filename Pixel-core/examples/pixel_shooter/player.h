#pragma once

#include "pixel.h"

#include "projectile.h"

class Player {
private:
	pixel::graphics::Window* _window;
	pixel::graphics::Layer* _layer;
	pixel::physics::World* _world;

	pixel::graphics::DynamicSprite* _sprite;
	std::vector<Projectile*> _projectiles;

	pixel::maths::vec2f _position;
	float _speed;

public:
	Player(float, float, pixel::graphics::Window* window, pixel::graphics::Layer* layer, pixel::physics::World* world);

	void update();
	void shoot(unsigned int);
	void contact();
	void reset();

	void setSpeed(float speed);
	pixel::maths::vec2f getPosition() const;
	pixel::maths::vec2f getVector(double, double) const;

	void defineBody();
};