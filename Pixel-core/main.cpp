#include "src/graphics/window.h"
#include "src/graphics/shader.h"
#include "src/graphics/buffers/buffer.h"
#include "src/graphics/buffers/indexbuffer.h"
#include "src/graphics/buffers/vertexarray.h"
#include "src/graphics/renderer2d.h"
#include "src/graphics/simplerenderer2d.h"
#include "src/graphics/renderable2d.h"
#include "src/maths/mat4.h"

int main() {

	using namespace pixel;
	using namespace graphics;
	using namespace maths;

	Window window("Pixel", 1600, 900);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	Shader shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0.0f, 16.0f, 0.0f, 9.0f, -1.0f, 1.0f);
	shader.setUniformMat4("pr_matrix", ortho);
	shader.setUniformMat4("ml_matrix", mat4::translation(vec3f(1,1,0)));

	Renderable2D sprites[] = {
		{ maths::vec3f(1.0f,  1.0f, 0.0f), maths::vec2f(2, 7), vec4f(0.5f, 0.2f, 0.6f, 1.0f), shader },
		{ maths::vec3f(4.0f,  1.0f, 0.0f), maths::vec2f(2, 7), vec4f(0.5f, 0.2f, 0.6f, 1.0f), shader },
		{ maths::vec3f(7.0f,  1.0f, 0.0f), maths::vec2f(2, 7), vec4f(0.5f, 0.2f, 0.6f, 1.0f), shader },
		{ maths::vec3f(10.0f, 1.0f, 0.0f), maths::vec2f(2, 7), vec4f(0.5f, 0.2f, 0.6f, 1.0f), shader },
		{ maths::vec3f(13.0f, 1.0f, 0.0f), maths::vec2f(2, 7), vec4f(0.5f, 0.2f, 0.6f, 1.0f), shader }
	};

	SimpleRenderer2D renderer;

	double mx, my;

	while(!window.closed()) {
		window.clear();

		window.getMousePos(mx, my);

		shader.setUniform2f("light_pos", vec2f((float)(mx * 16.0f / 1600.0f), (float)(9.0f - my * 9.0f / 900.0f)));

		for (int index = 0; index < 5; index++) {
			renderer.submit(&sprites[index]);
		}
		renderer.flush();

		window.update();
	}

	return 0;
}