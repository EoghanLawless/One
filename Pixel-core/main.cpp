#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/simplerenderer2d.h"
#include "src/graphics/batchrenderer2d.h"
#include "src/graphics/staticsprite.h"
#include "src/graphics/sprite.h"
#include "src/maths/maths.h"
#include "src/utils/timer.h"

#include "src/graphics/layers/flatlayer.h"

#include <time.h>

#define USE_BATCH_RENDERER 1

int main() {
	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 1600, 900);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();

	FlatLayer layer(&shader);

	for (float y = -8.0f; y < 8.0f; y += 0.5) {
		for (float x = -15.0f; x < 15.0f; x += 0.5) {
			layer.add(new Sprite(x, y, 0.45f, 0.45f, maths::vec4f(1.0f, 0.0f, 0.0f, 1.0f)));
		}
	}

	std::cout << "Sprite count: " << layer.count() << std::endl;

	double mx, my;

	Timer time;
	float timer = 0;
	int frames = 0;

	while(!window.closed()) {
		window.clear();
		window.getMousePos(mx, my);
		shader.setUniform2f("light_pos", vec2f((float)((mx - 800.0f) * 16.0f / 800.0f), (float)(9.0f - my * 9.0f / 450.0f)));

		layer.render();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f) {
			printf("%d fps\n", frames);
			timer = time.elapsed();
			frames = 0;
		}
	}

	return 0;
}