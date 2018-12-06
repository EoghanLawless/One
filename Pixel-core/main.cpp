#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include "src/graphics/buffers/vertexarray.h"
#include "src/graphics/batchrenderer2d.h"
#include "src/graphics/simplerenderer2d.h"
#include "src/graphics/sprite.h"
#include "src/graphics/staticsprite.h"
#include "src/maths/mat4.h"

#include <time.h>

#define USE_BATCH_RENDERER 1

int main() {

	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	srand(time(NULL));

	Window window("Pixel", 1600, 900);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);
	shader.setUniformMat4("pr_matrix", ortho);

	std::vector<Renderable2D*> sprites;

#if USE_BATCH_RENDERER
	BatchRenderer2D renderer;
#else
	SimpleRenderer2D renderer;
#endif

	float d = 0.095f;
	float s = d - 0.01f;

	for (float y = 0; y < 9.0f; y += d) {
		for (float x = 0; x < 16.0f; x += d) {
			float random = 0.2f + rand() % 1000 / 1000.0f;
#if USE_BATCH_RENDERER
			sprites.push_back(new Sprite(x, y, s, s, maths::vec4f(random, 0.2f, random, 1.0f)));
#else
			sprites.push_back(new StaticSprite(x, y, s, s, maths::vec4f(rand() % 1000 / 1000.0f, 0.0f, 0.8f, 1.0f), shader));
#endif
		}
	}

	std::cout << sprites.size() << std::endl;

	double mx, my;

	while(!window.closed()) {
		window.clear();
		window.getMousePos(mx, my);
		shader.setUniform2f("light_pos", vec2f((float)(mx * 16.0f / 1600.0f), (float)(9.0f - my * 9.0f / 900.0f)));


#if USE_BATCH_RENDERER
		renderer.begin();
		for (int index = 0; index < sprites.size(); index++) {
			renderer.submit(sprites[index]);
		}
		renderer.end();
		renderer.flush();
#else
		for (int index = 0; index < sprites.size(); index++) {
			renderer.submit(sprites[index]);
		}
		renderer.flush();
#endif

		window.update();
	}

	return 0;
}