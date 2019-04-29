#pragma once

#include "pixel.h"

#include "filter.h"
#include "player.h"

class Enemy {
private:
	pixel::graphics::Layer* _layer;
	pixel::physics::World* _world;

	Player* _player;

	pixel::graphics::DynamicSprite* _sprite;

	float _speed;

public:
	Enemy(float x, float y, pixel::graphics::Layer* layer, pixel::physics::World* world, Player* player) {
		_layer = layer;
		_world = world;
		_player = player;

		_speed = 1.5f;

		_sprite = new pixel::graphics::DynamicSprite(x, y, 1.0f, 1.0f, 0xFF2244BB);
		_sprite->body_definition.type = pixel::physics::DYNAMIC;
		_sprite->body_definition.linearDamping = 0.5f;
		_sprite->body_definition.fixedRotation = true; // REMOVE
		_sprite->createBody(*_world);
		_sprite->get()->setUserData(this);

		b2CircleShape circle;
		circle.m_radius = 0.5f;
		_sprite->fixture_definition.shape = &circle;
		_sprite->fixture_definition.density = 1.0f;
		_sprite->fixture_definition.friction = 0.1f;
		_sprite->fixture_definition.restitution = 0.0f;
		_sprite->fixture_definition.get()->filter.categoryBits = ENEMY;
		_sprite->createFixture(&_sprite->fixture_definition);

		_layer->add(_sprite);
	}

	void update() {
		pixel::maths::vec2f vector = getVector(_player->getPosition().x, _player->getPosition().y).normalise().mult(_speed);

		_sprite->applyForce(vector);
		_sprite->update();
	}
	void contact() {
		_sprite->colour = 0x77222222;
	}
	void reset() {
		_sprite->colour = 0xFF2244BB;
	}

	void setSpeed(float speed) {
		_speed = speed;
	}
	pixel::maths::vec2f getVector(double x, double y) const {
		return pixel::maths::vec2f(x - 0.5f - _sprite->position.x, y - 0.5f - _sprite->position.y);
	}
};