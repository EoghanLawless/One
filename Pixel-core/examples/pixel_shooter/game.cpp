#include "pixel.h"

#include "res/layers/dynamiclayer.h"

#include "contactlistener.h"
#include "enemymanager.h"
#include "player.h"
#include "enemy.h"

#define DEBUG 1

using namespace pixel;
using namespace graphics;
using namespace physics;
using namespace audio;
using namespace maths;

class Game : public Pixel {

private:
	float width, height;
	int pixel_w, pixel_h;
	double mouse_x, mouse_y;

	Window* window;
	SoundManager* sounds;

	Shader* shader_basic;
	Shader* shader_lighting;

	Layer* background_layer;
	Layer* hud_layer;
	DynamicLayer* dynamic_layer;

	Label* fps;
	Player* player;

	Texture* projectile;

	World* world;
	ContactListener* contact_listener;

	EnemyManager* enemies;
	std::vector<DynamicSprite*> dynamic_sprites;

	std::string _resource_dir;

public:
	Game() {
		if (DEBUG)
			_resource_dir = "examples/pixel_shooter/res";
		else
			_resource_dir = "res";
	}
	~Game() {
		delete shader_basic;

		delete background_layer;
		delete hud_layer;

		delete projectile;
	}


	void init() override {
		width = 32.0f;
		height = 18.0f;

		pixel_w = 1280;
		pixel_h = pixel_w / (width / height);
		window = createWindow("Basic Physics", pixel_w, pixel_h);
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

		shader_basic = new Shader(_resource_dir + "/shaders/basic.vs", _resource_dir + "/shaders/basic.fs");
		shader_lighting = new Shader(_resource_dir + "/shaders/lighting.vs", _resource_dir + "/shaders/lighting.fs");

		background_layer = new Layer(new BatchRenderer(), shader_lighting, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
		hud_layer = new Layer(new BatchRenderer(), shader_basic, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
		dynamic_layer = new DynamicLayer(new BatchRenderer(), shader_basic, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));

		sounds = new SoundManager(window->getHWND(), 44100, 15, 5, 8);
		sounds->add(new Sound("Jump", _resource_dir + "/sounds/jump.wav"));

		projectile = new Texture(_resource_dir + "/textures/red_ball.png");

		world = new World(vec2f(0.0f, 0.0f));

		contact_listener = new ContactListener;
		world->setContactListener(contact_listener);
		
		int rows = 18;
		int cols = 32;
		float tile_w = width / cols;
		float tile_h = height / rows;
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < cols; col++) {
				if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1) {
					DynamicSprite* s = new DynamicSprite((tile_w * col) - width / 2, (tile_h * row) - height / 2, tile_w, tile_h, 0xBB000000);
					s->createBody(*world);
					s->createFixture();
					dynamic_layer->add(s);
					dynamic_sprites.push_back(s);
				}
			}
		}


		player = new Player(0.0f, 0.0f, window, dynamic_layer, world);


		EnemyManager::enemies.push_back(new Enemy(-15.0f, 7.0f, dynamic_layer, world, player));
		EnemyManager::enemies.push_back(new Enemy(14.0f, 7.0f, dynamic_layer, world, player));
		EnemyManager::enemies.push_back(new Enemy(14.0f, -8.0f, dynamic_layer, world, player));
		EnemyManager::enemies.push_back(new Enemy(-15.0f, -8.0f, dynamic_layer, world, player));


		std::cout << "SDFGHJK£" << std::endl;


		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
		fps = new Label("000 fps", -15.5f, 8.1f, 0xFF00FF00);
		hud_layer->add(fps);
	}

	void tick() override {
		EnemyManager::enemies.push_back(new Enemy(-15.0f, 7.0f, dynamic_layer, world, player));
		EnemyManager::enemies.push_back(new Enemy(14.0f, 7.0f, dynamic_layer, world, player));
		EnemyManager::enemies.push_back(new Enemy(14.0f, -8.0f, dynamic_layer, world, player));
		EnemyManager::enemies.push_back(new Enemy(-15.0f, -8.0f, dynamic_layer, world, player));

		fps->text = std::to_string(getFrames()) + " fps";
		std::cout << getUpdates() << " ups, " << getFrames() << " fps, " << (background_layer->count() + dynamic_layer->count() + hud_layer->count()) << " sprites" << std::endl;
	}

	void update() override {
		window->getMousePos(mouse_x, mouse_y);
		projectPixelCoords(mouse_x, mouse_y, pixel_w, pixel_h, width, height);

		//if (window->mousePressed(GLFW_MOUSE_BUTTON_1)) {
		//	DynamicSprite* s = new DynamicSprite(mouse_x - 0.125f, mouse_y - 0.125f, 0.25f, 0.25f, 0xFF88FFFF);
		//	s->body_definition.type = DYNAMIC;
		//	s->body_definition.linearDamping = 0.3f;
		//	s->body_definition.gravityScale = 0.3f;
		//	s->createBody(*world);

		//	b2CircleShape circle;
		//	circle.m_radius = 0.125f;
		//	s->fixture_definition.shape = &circle;
		//	s->fixture_definition.density = 0.7f;
		//	s->fixture_definition.friction = 0.0f;
		//	s->fixture_definition.restitution = 0.5f;
		//	s->createFixture(&s->fixture_definition);
		//	background_layer->add(s);
		//	dynamic_sprites.push_back(s);
		//}

		//if (window->keyTyped(GLFW_KEY_I)) {
		//	DynamicSprite* s = new DynamicSprite(player->position.x + 0.65f, player->position.y + 1.8f, 0.5f, 0.5f, projectile);
		//	s->body_definition.type = DYNAMIC;
		//	s->body_definition.linearVelocity = vec2f(0.0f, 30.0f);
		//	s->body_definition.linearDamping = 0.3f;
		//	s->body_definition.gravityScale = 0.7f;
		//	s->createBody(*world);

		//	b2CircleShape circle;
		//	circle.m_radius = 0.25f;
		//	s->fixture_definition.shape = &circle;
		//	s->fixture_definition.density = 1.0f;
		//	s->fixture_definition.friction = 0.1f;
		//	s->fixture_definition.restitution = 0.2f;
		//	s->createFixture(&s->fixture_definition);
		//	background_layer->add(s);
		//	dynamic_sprites.push_back(s);
		//}


		world->step(TICK_INTERVAL / 10.0f, 16, 12);


		for (DynamicSprite* s : dynamic_sprites) {
			s->update();
		}

		player->update();
		player->shoot(10);

		enemies->update();


		shader_lighting->enable();
		shader_lighting->setUniform2f("light_pos", vec2f(player->getPosition().x + 0.5f, player->getPosition().y + 0.5f));
	}

	void render() override {
		background_layer->render();
		dynamic_layer->render();
		hud_layer->render();
	}
};

void killEnemy(Enemy&);

int main(int argc, char** argv) {

	Game game;
	game.start();

	return 0;
}

void killEnemy(Enemy& enemy) {
	enemy.~Enemy();

	//std::vector<Enemy*>::iterator iterator = EnemyManager::enemies.begin();

	//if(EnemyManager::enemies.size() > 1)
	//	EnemyManager::enemies.erase(iterator + findElement(EnemyManager::enemies, &enemy));
}
