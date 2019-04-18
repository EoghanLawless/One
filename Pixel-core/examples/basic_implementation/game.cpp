#include "pixel.h"

#include "res/layers/dynamiclayer.h"

#define DEBUG 1

using namespace pixel;
using namespace graphics;
using namespace audio;

class Game : public Pixel {

private:
	Window* window;
	SoundManager* sounds;

	Shader* shader_greyscale;
	Shader* shader_basic;

	Layer* background_layer;
	Layer* character_layer;
	Layer* hud_layer;
	DynamicLayer* projectiles_layer;

	Label* fps;
	Sprite* player;

	Texture* projectile;

	std::string _resource_dir;

public:
	Game() {
		if (DEBUG)
			_resource_dir = "examples/basic_implementation/res";
		else
			_resource_dir = "res";
	}
	~Game() {
		delete shader_basic;
		delete shader_greyscale;

		delete background_layer;
		delete character_layer;
		delete hud_layer;

		delete projectile;
	}


	void init() override {
		window = createWindow("Basic Implementation", 960, 540);
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);


		projectile = new Texture(_resource_dir + "/textures/red_ball.png");


		sounds = new SoundManager(window->getHWND(), 44100, 15, 5, 8);
		sounds->add(new Sound("Jump", _resource_dir + "/sounds/jump.wav"));


		shader_greyscale = new Shader(_resource_dir + "/shaders/greyscale.vs", _resource_dir + "/shaders/greyscale.fs");
		shader_basic = new Shader(_resource_dir + "/shaders/basic.vs", _resource_dir + "/shaders/basic.fs");

		background_layer = new Layer(new BatchRenderer(), shader_greyscale, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		character_layer = new Layer(new BatchRenderer(), shader_basic, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		hud_layer = new Layer(new BatchRenderer(), shader_basic, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		projectiles_layer = new DynamicLayer(new BatchRenderer(), shader_basic, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));


		Texture* textures[] = {
			new Texture(_resource_dir + "/textures/test.png"),
			new Texture(_resource_dir + "/textures/test_white.png"),
			new Texture(_resource_dir + "/textures/test_red.png"),
			new Texture(_resource_dir + "/textures/test_green.png"),
			new Texture(_resource_dir + "/textures/test_blue.png")
		};
		unsigned int colours[] = {
			0xFF999999,
			0xFFFFFFFF,
			0xFFFF9999,
			0xFF99FF99,
			0xFF9999FF
		};

		float pad = 2.0f;
		float size = 0.8f;
		float gap = 0.0f;
		
		for (float y = -9.0f + pad; y < 9.0f - pad; y += size) {
			for (float x = -16.0f + pad; x < 16.0f - pad; x += size) {
				background_layer->add(new Sprite(x, y, size - gap, size - gap, colours[rand() % 5]));
				//background->add(new Sprite(x, y, size - gap, size - gap, textures[rand() % 5]));
			}
		}


		player = new Sprite(-1.5f, -1.5f, 3, 3, new Texture(_resource_dir + "/textures/blue_ball.png"));
		character_layer->add(player);

		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
		fps = new Label("000 fps", -15.5f, 8.1f, 0xFF00FF00);
		hud_layer->add(fps);
	}

	void tick() override {
		fps->text = std::to_string(getFrames()) + " fps";
		std::cout << getUpdates() << " ups, " << getFrames() << " fps, " << (background_layer->count() + character_layer->count() + projectiles_layer->count() + hud_layer->count()) << " sprites" << std::endl;
	}

	void update() override {

		float speed = 0.25f;

		if (window->keyPressed(GLFW_KEY_SPACE)) {
			Sprite* s = new Sprite(player->position.x + 1.2f, player->position.y + 1.2f, 0.6f, 0.6f, 0xFFFF0000);
			projectiles_layer->add(s);
		}
		if (window->mouseClicked(GLFW_MOUSE_BUTTON_1)) {
			sounds->play("Jump");
			Sprite* s = new Sprite(player->position.x + 1.0f, player->position.y + 1.0f, 1.0f, 1.0f, projectile);
			projectiles_layer->add(s);
		}

		if (window->keyPressed(GLFW_KEY_W))
			player->position.y += speed;
		else if (window->keyPressed(GLFW_KEY_S))
			player->position.y -= speed;

		if (window->keyPressed(GLFW_KEY_A))
			player->position.x -= speed;
		else if (window->keyPressed(GLFW_KEY_D))
			player->position.x += speed;


		for (int index = 0; index < projectiles_layer->count(); index++) {
			projectiles_layer->get(index)->position.y += 0.3f;

			if (projectiles_layer->get(index)->position.y >= 9.0f)
				projectiles_layer->remove(projectiles_layer->get(index));
		}


		//double x, y;
		//window->getMousePos(x, y);
		//shader->setUniform2f("light_pos", maths::vec2f((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
	}

	void render() override {
		background_layer->render();
		character_layer->render();
		projectiles_layer->render();
		hud_layer->render();
	}
};


int main(int argc, char** argv) {

	Game game;
	game.start();

	return 0;
}
