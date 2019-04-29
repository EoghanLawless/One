#pragma once

#include "pixel.h"

#include "filter.h"
#include "enemy.h"

class Projectile {
private:
	pixel::graphics::DynamicSprite* _sprite;

	bool _alive;

	float _size = 0.2f;

public:
	Projectile(pixel::maths::vec2f position, pixel::maths::vec2f vector, pixel::physics::World* world) {
		_alive = true;

		_sprite = new pixel::graphics::DynamicSprite(position.x + vector.x / 2, position.y + vector.y / 2, _size, _size, 0xFF88FFFF);
		_sprite->body_definition.type = pixel::physics::DYNAMIC;
		_sprite->body_definition.linearDamping = 0.3f;
		_sprite->body_definition.bullet = true;
		_sprite->createBody(*world);
		_sprite->get()->setUserData(this);

		b2CircleShape circle;
		circle.m_radius = 0.125f;
		_sprite->fixture_definition.shape = &circle;
		_sprite->fixture_definition.density = 0.7f;
		_sprite->fixture_definition.friction = 0.0f;
		_sprite->fixture_definition.restitution = 0.5f;
		_sprite->fixture_definition.get()->filter.categoryBits = PROJECTILE;
		_sprite->createFixture(&_sprite->fixture_definition);

		_sprite->applyImpulse(vector);
	}
	~Projectile() {
		_sprite->colour = 0x00FFFFFF;
		_alive = false;
	}

	void update() {
		if(_alive)
			_sprite->update();
	}
	void contact() {
		_alive = false;
	}

	pixel::graphics::DynamicSprite* getSprite() const {
		return _sprite;
	}
};