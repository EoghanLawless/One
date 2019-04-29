#pragma once

#include "pixel.h"

#include "enemy.h"

class EnemyManager {
private:
	pixel::graphics::Layer* _layer;
	pixel::physics::World* _world;

	Player* _player;

	float _speed;

public:
	static std::vector<Enemy*> enemies;

	EnemyManager(float x, float y, pixel::graphics::Layer* layer, pixel::physics::World* world, Player* player) {
		_layer = layer;
		_world = world;
		_player = player;

		_speed = 1.5f;
	}

	void update() {
		for (std::vector<Enemy*>::iterator iterator = enemies.begin(); iterator != enemies.end(); ++iterator) {
			(*iterator)->update();
		}
	}
};

std::vector<Enemy*> EnemyManager::enemies;