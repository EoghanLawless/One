#pragma once

#include "renderable.h"

#include "physics/physics.h"

namespace pixel {
	namespace graphics {

		class DynamicSprite : public Renderable {
		private:
			double _w, _h;
			physics::Body* _body;

			void define(double, double, double, double);

		public:
			DynamicSprite(float, float, float, float, const unsigned int);
			DynamicSprite(maths::vec2f, maths::vec2f, const unsigned int);
			DynamicSprite(maths::vec4f, const unsigned int);

			DynamicSprite(float, float, float, float, Texture*);
			DynamicSprite(maths::vec2f, maths::vec2f, Texture*);
			DynamicSprite(maths::vec4f, Texture*);

			physics::BodyDef body_definition;
			physics::FixtureDef fixture_definition;
			b2PolygonShape shape;

			void createBody(physics::World&);
			void createFixture();
			void createFixture(physics::FixtureDef* def);
			void createFixture(const b2Shape* shape, float density);

			maths::vec2f applyForce(const maths::vec2f&);
			maths::vec2f applyImpulse(const maths::vec2f&);

			void update();

			inline physics::Body* get() const { return _body; }
		};

	}
}