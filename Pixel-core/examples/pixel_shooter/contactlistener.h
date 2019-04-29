#pragma once

#include "pixel.h"

#include "filter.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"


void killEnemy(Enemy&);

class ContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) override {
		b2Filter filter_a = contact->GetFixtureA()->GetFilterData();
		void* user_data_a = contact->GetFixtureA()->GetBody()->GetUserData();

		b2Filter filter_b = contact->GetFixtureB()->GetFilterData();
		void* user_data_b = contact->GetFixtureB()->GetBody()->GetUserData();

		if (user_data_a && user_data_b) {
			if ((filter_a.categoryBits == PLAYER || filter_a.categoryBits == ENEMY) &&
				(filter_b.categoryBits == PLAYER || filter_b.categoryBits == ENEMY)) {

				//std::cout << "PLAYER DEAD" << std::endl;
			}

			if (filter_a.categoryBits == ENEMY && filter_b.categoryBits == PROJECTILE) {
				killEnemy(*static_cast<Enemy*>(user_data_a));
				static_cast<Projectile*>(user_data_b)->~Projectile();
				std::cout << "PLAYER DEAD" << std::endl;
			} else if (filter_a.categoryBits == PROJECTILE && filter_b.categoryBits == ENEMY) {
				static_cast<Projectile*>(user_data_a)->~Projectile();
				killEnemy(*static_cast<Enemy*>(user_data_b));
				std::cout << "PLAYER DEAD" << std::endl;
			}


			//if (filter_a.categoryBits == PROJECTILE || filter_b.categoryBits == PROJECTILE)
			//	std::cout << "PROJECTILE DEAD" << std::endl;
		}

		//if(user_data_a)
		//	if (filter_a.categoryBits == PROJECTILE)
		//		std::cout << "A: PROJECTILE" << std::endl;

		//if(user_data_b)
		//	if (filter_b.categoryBits == PROJECTILE)
		//		std::cout << "B: PROJECTILE" << std::endl;
	}
};