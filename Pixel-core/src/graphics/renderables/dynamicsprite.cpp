#include "dynamicsprite.h"

namespace pixel {
	namespace graphics {

		DynamicSprite::DynamicSprite(float x, float y, float w, float h, const unsigned int colour) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), colour), _w(w), _h(h) {
			define(x, y, w, h);
		}
		DynamicSprite::DynamicSprite(maths::vec2f pos, maths::vec2f size, const unsigned int colour) : Renderable(maths::vec3f(pos.x, pos.y, 0), maths::vec2f(size.x, size.y), colour), _w(size.x), _h(size.y) {
			define(pos.x, pos.y, size.x, size.y);
		}
		
		DynamicSprite::DynamicSprite(float x, float y, float w, float h, Texture* texture) : Renderable(maths::vec3f(x, y, 0), maths::vec2f(w, h), 0xFFFFFFFF), _w(w), _h(h) {
			_texture = texture;
			define(x, y, w, h);
		}
		DynamicSprite::DynamicSprite(maths::vec2f pos, maths::vec2f size, Texture* texture) : Renderable(maths::vec3f(pos.x, pos.y, 0), maths::vec2f(size.x, size.y), 0xFFFFFFFF), _w(size.x), _h(size.y) {
			_texture = texture;
			define(pos.x, pos.y, size.x, size.y);
		}

		DynamicSprite::~DynamicSprite() {
			_body->~Body();
		}

		void DynamicSprite::createBody(physics::World& world) {
			_body = world.createBody(&body_definition);
		}
		void DynamicSprite::createFixture() {
			_body->createFixture(&shape, 0.0f);
		}
		void DynamicSprite::createFixture(physics::FixtureDef* def) {
			_body->createFixture(def);
		}
		void DynamicSprite::createFixture(const b2Shape* shape, float density) {
			_body->createFixture(shape, density);
		}

		maths::vec2f DynamicSprite::applyForce(const maths::vec2f& f) {
			_body->get()->ApplyForceToCenter(b2Vec2(f.x, f.y), true);
			b2Vec2 v = _body->get()->GetLinearVelocity();
			return maths::vec2f(v.x, v.y);
		}
		maths::vec2f DynamicSprite::applyImpulse(const maths::vec2f& f) {
			_body->get()->ApplyLinearImpulse(b2Vec2(f.x, f.y), b2Vec2(position.x, position.y), true);
			b2Vec2 v = _body->get()->GetLinearVelocity();
			return maths::vec2f(v.x, v.y);
		}

		void DynamicSprite::update() {
			position = maths::vec3f(_body->getPosition().x - _w / 2, _body->getPosition().y - _h / 2, 0.0f);
		}

		void DynamicSprite::define(double x, double y, double w, double h) {
			shape.SetAsBox(w / 2, h / 2);
			body_definition.position.set(x + w / 2, y + h / 2);
			fixture_definition.shape = &shape;
		}

	}
}