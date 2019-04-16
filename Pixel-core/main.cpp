
#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/renderers/batchrenderer2d.h"
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

	int width = 1200;
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

	GLint textureIds[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	shader->enable();
	shader->setUniform1iv("textures", textureIds, 10);
	shader->setUniformMat4("pr_matrix", mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));


	FontManager::add(new Font("Raleway", "res/fonts/Raleway-Medium.ttf", 32));
	Group* textGroup = new Group(maths::mat4::translation(vec3f(-16.0f, 9.0f, 0.0f)));
	Label* fps = new Label("000", 0.5f, -0.62f, "Courier New", 16, 0xFF00FF00);
	textGroup->add(new Sprite(0.0f, 0.0f, 2.0f, -1.0f, 0xFF888888));
	textGroup->add(fps);
	layer.add(textGroup);


	double mx, my;

	Timer time;
	float timer = 0;
	unsigned int frames = 0;

	cs_playing_sound_t* sound = nullptr;
	 
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


		if (window.keyTyped(GLFW_KEY_W)) {
			std::cout << "Play 'Wilson'." << std::endl;
			sm.play("Wilson");
		}
		if (window.keyTyped(GLFW_KEY_J)) {
			std::cout << "Play 'Jump'." << std::endl;
			sm.play("Jump");
		}

		frames++;
		if (time.elapsed() - timer > 1.0f) {
			timer += 1.0f;
			fps->text = std::to_string(frames) + "";
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	for (int i = 0; i < 5; i++)
		delete textures[i];

	return 0;
}
