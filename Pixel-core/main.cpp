#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/batchrenderer2d.h"
#include "src/graphics/sprite.h"
#include "src/maths/maths.h"
#include "src/utils/timer.h"

#include "src/graphics/layers/flatlayer.h"
#include "src/graphics/layers/group.h"

#include "src/graphics/texture.h"

#include <time.h>

int main() {
	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 1600, 900);

	Shader* shader1 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");

	FlatLayer layer1(shader1);

	float pad = 2.0f;
	float size = 2.0f;
	float gap = 0.1f;

	for (float y = -9.0f + pad; y < 9.0f - pad; y += size) {
		for (float x = -16.0f + pad; x < 16.0f - pad; x += size) {
			float value = rand() % 1000 / 1000.0f;
			layer1.add(new Sprite(x, y, size - gap, size - gap, vec4f(value, value, value, 1)));
		}
	}


	glActiveTexture(GL_TEXTURE0);
	Texture texture("test1.png");
	texture.bind();

	shader1->enable();
	shader1->setUniform1i("tex", 0);
	shader1->setUniformMat4("pr_matrix", mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

	std::cout << layer1.count() << std::endl;


	double mx, my;

	Timer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed()) {
		window.clear();
		window.getMousePos(mx, my);

		//mat4 mat = mat4::translation(vec3f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f), 0));
		//mat4 mat = mat4::rotation(time.elapsed() * 100.0f, vec3f(0, 0, 1));
		//mat *= mat4::translation(vec3f(-(float)((mx - 800.0f) * 16.0f / 800.0f), -(float)(9.0f - my * 9.0f / 450.0f), 0));

		//shader1->setUniformMat4("ml_matrix", mat);
		shader1->setUniform2f("light_pos", vec2f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f)));
		layer1.render();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f) {
			timer += 1.0f;
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	return 0;
}