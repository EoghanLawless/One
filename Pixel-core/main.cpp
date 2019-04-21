
#if 0
#include <iostream>
#include <stdio.h>


#include "Box2D/Box2D.h"

#include "pixel.h"
#include "physics/body.h"
#include "physics/fixture.h"
#include "physics/world.h"


using namespace pixel;
using namespace graphics;
using namespace physics;
using namespace maths;


int main(int argc, char** argv) {

	//double floor_x = 0.0f;
	//double floor_y = -8.0f;
	//double floor_w = 20.0f;
	//double floor_h = 2.0f;





	//Sprite* floor = new Sprite(-floor_w / 2, floor_y, floor_w, floor_h, 0xAA0000FF);
	//layer.add(floor);


	//B2_NOT_USED(argc);
	//B2_NOT_USED(argv);

	//int i = 0;

	//World world(vec2f(0.0f, 9.8f));
	//std::cout << i++ << " " << world.getGravity() << std::endl; // 0

	//BodyDef groundDef;
	//groundDef.position.set(floor_x, floor_y);
	//groundDef.set();
	//std::cout << i++ << " " << world.getB2World().IsLocked() << std::endl; // 1

	//Body* groundBody = world.createBody(&groundDef);
	////std::cout << i++ << " " << world.getB2World().IsLocked() << std::endl; // 2

	//b2PolygonShape groundBox;
	//groundBox.SetAsBox(floor_w / 2, floor_h / 2);
	//groundBody->createFixture(&groundBox, 0.0f);
	//std::cout << i++ << " " << world.getB2World().IsLocked() << std::endl; // 3


	//BodyDef bodyDef;
	//bodyDef.type = b2_dynamicBody;
	//bodyDef.position.set(0.0f, 1.0f);
	//bodyDef.set();

	//Body* body = world.createBody(&bodyDef);

	//b2PolygonShape dynamicBox;
	//dynamicBox.SetAsBox(1.0f, 1.0f);

	//b2FixtureDef fixtureDef;
	//fixtureDef.shape = &dynamicBox;
	//fixtureDef.density = 1.0f;
	//fixtureDef.friction = 0.3f;

	//body->createFixture(&fixtureDef);


	//float32 timeStep = 1.0f / 240.0f;
	//int32 velocityIterations = 8;
	//int32 positionIterations = 8;



	Window window("Title", 800, 450);
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	Shader shader("res/shaders/basic.vs", "res/shaders/basic.fs");
	Layer layer(new BatchRenderer(), &shader, mat4::orthographic(-32.0f, 32.0f, -18.0f, 18.0f, -1.0f, 1.0f));

	DynamicSprite* s = new DynamicSprite(0, 0, 4, 4, 0xAA00FF00);
	layer.add(s);



	World world(vec2f(0.0f, 10.0f));

	BodyDef def;
	def.type = DYNAMIC;

	Body* body = world.createBody(&def);

	b2PolygonShape shape;
	shape.SetAsBox(10, 3);

	FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.3f;

	body->createFixture(&fixture);

	float32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	double x, y;

	while (!window.closed()) {
		window.clear();

		window.getMousePos(x, y);
		projectPixelCoords(x, y, 800, 450, 64.0f, 36.0f);

		world.step(timeStep, velocityIterations, positionIterations);

		vec2f position = body->getPosition();
		float32 angle = body->getAngle();

		s->position = vec3f(position.x, position.y, 0.0f);

		layer.render();

		if (window.mouseClicked(GLFW_MOUSE_BUTTON_1)) {
			layer.add(new Sprite(x - 0.5f, y - 0.5f, 1.0f, 1.0f, 0xFF00FF00));
			std::cout << x << ", " << y << std::endl;
		}

		window.update();
		window.poll();

	}

	world.destroyBody(body);

	return 0;
}


#elif 0

#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/renderers/batchrenderer.h"
#include "graphics/layers/flatlayer.h"
#include "graphics/layers/group.h"
#include "graphics/renderables/label.h"
#include "graphics/renderables/sprite.h"

#include "graphics/fontmanager.h"
#include "audio/soundmanager.h"

#include "maths/maths.h"
#include "utils/timer.h"


int main() {
	using namespace pixel;
	using namespace graphics;
	using namespace audio;
	using namespace maths;

	float ratio = 16.0f / 9.0f;

	int width = 960;
	int height = width / ratio;

	Window window("Pixel", width, height);

	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);


	SoundManager sm(window.getHWND(), 44100, 15, 5, 7);
	sm.add(new Sound("Wilson", "res/sounds/wilson16.wav"));
	sm.add(new Sound("Jump", "res/sounds/jump.wav"));


	Shader* shader = new Shader("res/shaders/lighting.vs", "res/shaders/lighting.fs");
	FlatLayer layer(shader);

	Texture* textures[] = {
		new Texture("res/textures/test.png"),
		new Texture("res/textures/test_white.png"),
		new Texture("res/textures/test_red.png"),
		new Texture("res/textures/test_green.png"),
		new Texture("res/textures/test_blue.png")
	};

	float pad = 2.0f;
	float size = 0.8f;
	float gap = 0.0f;

	for (float y = -9.0f + pad; y < 9.0f - pad; y += size) {
		for (float x = -16.0f + pad; x < 16.0f - pad; x += size) {
			float value = rand() % 1000 / 1000.0f;
			//layer.add(new Sprite(x, y, size - gap, size - gap, vec4f(value, value, value, 1)));
			layer.add(new Sprite(x, y, size - gap, size - gap, textures[rand() % 5]));
		}
	}
	std::cout << layer.count() << std::endl;


	FontManager::add(new Font("Raleway", "res/fonts/Raleway-Medium.ttf", 32));
	FontManager::get()->setScale(width / 32.0f, height / 18.0f);
	Group* textGroup = new Group(maths::mat4::translation(vec3f(-16.0f, 9.0f, 0.0f)));
	Label* fps = new Label("000", 0.5f, -0.62f, "Courier New", 16, 0xFF00FF00);
	textGroup->add(new Sprite(0.0f, 0.0f, 2.0f, -1.0f, 0xFF888888));
	textGroup->add(fps);
	layer.add(textGroup);


	double mx, my;

	unsigned int frames = 0;
	Timer timer = Timer().start();

	while (!window.closed()) {

		window.clear();
		window.getMousePos(mx, my);

		//mat4 mat = mat4::translation(vec3f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f), 0));
		//mat4 mat = mat4::rotation(time.elapsed() * 100.0f, vec3f(0, 0, 1));
		//mat *= mat4::translation(vec3f(-(float)((mx - 800.0f) * 16.0f / 800.0f), -(float)(9.0f - my * 9.0f / 450.0f), 0));

		shader->enable();
		shader->setUniform2f("light_pos", vec2f((float)(mx * 32.0f / window.getWidth() - 16.0f), (float)(9.0f - my * 18.0f / window.getHeight())));
		layer.render();

		window.update();
		window.poll();


		if (window.keyTyped(GLFW_KEY_W)) {
			std::cout << "Play 'Wilson'." << std::endl;
			sm.play("Wilson");
		}
		if (window.keyTyped(GLFW_KEY_J)) {
			std::cout << "Play 'Jump'." << std::endl;
			sm.play("Jump");
		}

		if (window.keyTyped(GLFW_KEY_U)) {
			std::cout << "Increase volume.: ";
			if (sm.get("Jump")) {
				float volume = sm.get("Jump")->getVolume();
				sm.setVolume("Jump", volume + 0.5f);
				std::cout << (volume + 0.5f) << std::endl;
			}
		}
		if (window.keyTyped(GLFW_KEY_D)) {
			std::cout << "Decrease volume.: ";
			if (sm.get("Jump")) {
				float volume = sm.get("Jump")->getVolume();
				sm.setVolume("Jump", volume - 0.5f);
				std::cout << (volume - 0.5f) << std::endl;
			}
		}

		frames++;
		if (timer.elapsedSeconds() >= 1.0f) {
			fps->text = std::to_string(frames) + "";
			std::cout << frames << " fps" << std::endl;
			frames = 0;
			timer.restart();
		}
	}

	for (int i = 0; i < 5; i++)
		delete textures[i];

	return 0;
}
#endif