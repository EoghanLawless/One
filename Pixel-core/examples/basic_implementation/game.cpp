#include "pixel.h"

using namespace pixel;
using namespace graphics;
using namespace audio;

class Game : public Pixel {

private:
	Window* window;
	SoundManager* sounds;

	Shader* shader_greyscale;
	Shader* shader_basic;

	Layer* background;
	Layer* player;
	Layer* hud;

	Label* fps;
	Sprite* sprite;

public:
	Game() { }
	~Game() {
		delete shader_basic;
		delete shader_greyscale;

		delete background;
		delete player;
		delete hud;
	}


	void init() override {
		window = createWindow("Basic Implementation", 960, 540);
		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);


		sounds = new SoundManager(window->getHWND(), 44100, 15, 5, 8);
		sounds->add(new Sound("Jump", "examples/basic_implementation/res/sounds/jump.wav"));


		shader_greyscale = new Shader("examples/basic_implementation/res/shaders/greyscale.vs", "examples/basic_implementation/res/shaders/greyscale.fs");
		shader_basic = new Shader("examples/basic_implementation/res/shaders/basic.vs", "examples/basic_implementation/res/shaders/basic.fs");

		background = new Layer(new BatchRenderer(), shader_greyscale, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		player = new Layer(new BatchRenderer(), shader_basic, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
		hud = new Layer(new BatchRenderer(), shader_basic, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));


		Texture* textures[] = {
			new Texture("examples/basic_implementation/res/textures/test.png"),
			new Texture("examples/basic_implementation/res/textures/test_white.png"),
			new Texture("examples/basic_implementation/res/textures/test_red.png"),
			new Texture("examples/basic_implementation/res/textures/test_green.png"),
			new Texture("examples/basic_implementation/res/textures/test_blue.png")
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
				background->add(new Sprite(x, y, size - gap, size - gap, colours[rand() % 5]));
				//background->add(new Sprite(x, y, size - gap, size - gap, textures[rand() % 5]));
			}
		}
		

		sprite = new Sprite(-1.5f, -1.5f, 3, 3, new Texture("examples/basic_implementation/res/textures/blue_ball.png"));
		player->add(sprite);

		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
		fps = new Label("000 fps", -15.5f, 8.1f, 0xFF00FF00);
		hud->add(fps);
	}

	void tick() override {
		fps->text = std::to_string(getFrames()) + " fps";
		std::cout << getUpdates() << " ups, " << getFrames() << " fps" << std::endl;
	}

	void update() override {

		float speed = 0.25f;

		if (window->keyTyped(GLFW_KEY_SPACE)) {
			sounds->play("Jump");
		}
		if (window->mouseClicked(GLFW_MOUSE_BUTTON_1)) {
			sounds->play("Jump");
		}

		if (window->keyPressed(GLFW_KEY_UP))
			sprite->position.y += speed;
		else if (window->keyPressed(GLFW_KEY_DOWN))
			sprite->position.y -= speed;

		if (window->keyPressed(GLFW_KEY_LEFT))
			sprite->position.x -= speed;
		else if (window->keyPressed(GLFW_KEY_RIGHT))
			sprite->position.x += speed;


		//double x, y;
		//window->getMousePos(x, y);
		//shader->setUniform2f("light_pos", maths::vec2f((float)(x * 32.0f / window->getWidth() - 16.0f), (float)(9.0f - y * 18.0f / window->getHeight())));
	}

	void render() override {
		background->render();
		player->render();
		hud->render();
	}
};


int main(int argc, char** argv) {

	Game game;
	game.start();

	return 0;
}
