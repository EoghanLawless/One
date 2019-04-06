#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/batchrenderer2d.h"
#include "src/graphics/sprite.h"
#include "src/maths/maths.h"
#include "src/utils/timer.h"

#include "src/graphics/layers/flatlayer.h"
#include "src/graphics/layers/group.h"

#include <time.h>

int main() {
	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 1600, 900);
	// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);

	Shader* shader1 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");

	FlatLayer layer1(shader1);
	for (float y = -9.0f; y < 9.0f; y += 0.5) {
		for (float x = -16.0f; x < 16.0f; x += 0.5) {
			layer1.add(new Sprite(x, y, 0.45f, 0.45f, maths::vec4f(rand() % 1000 / 1000.0f, 0, 1, 1)));
		}
	}

	Shader* shader2 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");

	FlatLayer layer2(shader2);
	Group* group = new Group(maths::mat4::translation(maths::vec3f(-5, -5, 0)));
	group->add(new Sprite(0, 0, 5, 5, maths::vec4f(1, 0, 1, 1)));
	group->add(new Sprite(0.5, 0.5, 4, 4, maths::vec4f(0, 1, 1, 1)));
	layer2.add(group);


	double mx, my;

	Timer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed()) {
		window.clear();
		window.getMousePos(mx, my);

		//mat4 mat = mat4::translation(vec3f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f), 0));
		//mat *= mat4::rotation(time.elapsed() * 100.0f, vec3f(0, 0, 1));
		//mat *= mat4::translation(vec3f(-(float)((mx - 800.0f) * 16.0f / 800.0f), -(float)(9.0f - my * 9.0f / 450.0f), 0));

		shader1->enable();
		//shader1->setUniformMat4("ml_matrix", mat);
		shader1->setUniform2f("light_pos", vec2f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f)));
		layer1.render();

		shader2->enable();
		shader2->setUniform2f("light_pos", vec2f(0, -2));
		layer2.render();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	return 0;
}