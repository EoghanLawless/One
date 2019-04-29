#include "player.h"

#include "filter.h"
#include "projectile.h"

Player::Player(float x, float y, pixel::graphics::Window* window, pixel::graphics::Layer* layer, pixel::physics::World* world) {
	_window = window;
	_layer = layer;
	_world = world;
	_position = pixel::maths::vec2f(x, y);

	_speed = 3.0f;

	_sprite = new pixel::graphics::DynamicSprite(x, y, 1.0f, 1.0f, 0xFFAA8822);
	_layer->add(_sprite);

	defineBody();
}

void Player::update() {
	if (_window->keyPressed(GLFW_KEY_W))
		_sprite->applyForce(pixel::maths::vec2f(0.0f, _speed));
	if (_window->keyPressed(GLFW_KEY_A))
		_sprite->applyForce(pixel::maths::vec2f(-_speed, 0.0f));
	if (_window->keyPressed(GLFW_KEY_S))
		_sprite->applyForce(pixel::maths::vec2f(0.0f, -_speed));
	if (_window->keyPressed(GLFW_KEY_D))
		_sprite->applyForce(pixel::maths::vec2f(_speed, 0.0f));

	_sprite->update();

	for (pixel::graphics::DynamicSprite* projectile : _projectiles) {
		projectile->update();
	}
}
void Player::shoot(unsigned int fire_rate) {
	double mouse_x, mouse_y;
	_window->getMousePos(mouse_x, mouse_y);
	projectPixelCoords(mouse_x, mouse_y, _window->getWidth(), _window->getHeight(), 32.0f, 18.0f);

	pixel::maths::vec2f vector = getVector(mouse_x, mouse_y).normalise();


	if (_window->mousePressed(GLFW_MOUSE_BUTTON_1)) {

		static int updates = 0;
		if (updates >= 60 / fire_rate) {
			Projectile* projectile = new Projectile(pixel::maths::vec2f(0.5f + _sprite->position.x + vector.x, 0.5f + _sprite->position.y + vector.y), vector, _world);

			_layer->add(projectile->getSprite());
			_projectiles.push_back(projectile->getSprite());

			updates = 0;
		} else updates++;
	}
}
void Player::contact() {
	std::cout << "PLAYER" << std::endl;
}

void Player::setSpeed(float speed) {
	_speed = speed;
}
pixel::maths::vec2f Player::getPosition() const {
	return pixel::maths::vec2f(_sprite->position.x, _sprite->position.y);
}
pixel::maths::vec2f Player::getVector(double x, double y) const {
	return pixel::maths::vec2f(x - 0.5f - _sprite->position.x, y - 0.5f - _sprite->position.y);
}

void Player::defineBody() {
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
	_sprite->fixture_definition.get()->filter.categoryBits = PLAYER;
	_sprite->createFixture(&_sprite->fixture_definition);
}