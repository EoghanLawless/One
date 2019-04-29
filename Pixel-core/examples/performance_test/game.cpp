//#include "pixel.h"
//
//#define DEBUG 1
//
//#define USE_BATCH_RENDERER 1
//#define NUM_SPRITES 10000
//
//#define TICK_DELAY 3
//#define NUM_SAMPLES 10
//
//using namespace pixel;
//using namespace graphics;
//using namespace maths;
//
//class Game : public Pixel {
//
//private:
//	float width, height;
//
//	Window* window;
//	Shader* shader;
//	Layer* layer;
//
//#if !USE_BATCH_RENDERER
//	SimpleRenderer renderer;
//	std::vector<Renderable*> sprites;
//#endif
//
//	std::string _resource_dir;
//
//public:
//	Game() {
//		if (DEBUG)
//			_resource_dir = "examples/performance_test/res";
//		else
//			_resource_dir = "res";
//	}
//	~Game() {
//		delete shader;
//	}
//
//
//	void init() override {
//		width = 32.0f;
//		height = 18.0f;
//
//		int pixel_w = 1200;
//		int pixel_h = pixel_w / (width / height);
//		window = createWindow("Performance Test", pixel_w, pixel_h);
//
//#if USE_BATCH_RENDERER
//		shader = new Shader(_resource_dir + "/shaders/basic.vs", _resource_dir + "/shaders/basic.fs");
//		layer = new Layer(new BatchRenderer(), shader, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
//#else
//		shader = new Shader(_resource_dir + "/shaders/simple.vs", _resource_dir + "/shaders/simple.fs");
//		shader->enable();
//		shader->setUniformMat4("pr_matrix", mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
//#endif
//
//		unsigned int colours[] = {
//			0xFF999999,
//			0xFFFFFFFF,
//			0xFFFF9999,
//			0xFF99FF99,
//			0xFF9999FF
//		};
//
//		int sprite_counter = 0;
//		float sprite_size = 0.1f;
//		for (int row = (height / sprite_size); row > 0; row--) {
//			for (int col = 0; col < (width / sprite_size) && sprite_counter++ < NUM_SPRITES; col++) {
//#if USE_BATCH_RENDERER
//				layer->add(new Sprite((sprite_size * col) - width / 2, (sprite_size * row) - height / 2, sprite_size, sprite_size, colours[rand() % 5]));
//#else
//				sprites.push_back(new StaticSprite((sprite_size * col) - width / 2, (sprite_size * row) - height / 2, sprite_size, sprite_size, maths::vec4f(rand() % 1000 / 1000.0f, 0.0f, 0.8f, 1.0f), *shader));
//#endif
//			}
//		}
//	}
//
//	void tick() override {
//		static double frames = 0;
//		static int tick_counter = 0;
//		if (++tick_counter > TICK_DELAY && tick_counter < NUM_SAMPLES + TICK_DELAY) {
//			frames += getFrames();
//		}
//		else if (tick_counter == NUM_SAMPLES + TICK_DELAY)
//			std::cout << "# " << frames / NUM_SAMPLES << std::endl;
//
//		std::cout << tick_counter << ": " << getUpdates() << " ups, " << getFrames() << " fps, " << NUM_SPRITES << " sprites" << std::endl;
//	}
//
//	void update() override { }
//
//	void render() override {
//#if USE_BATCH_RENDERER
//		layer->render();
//#else
//		for (const auto& s : sprites) {
//			renderer.submit(s);
//		}
//		renderer.flush();
//#endif
//	}
//};
//
//
//int main(int argc, char** argv) {
//
//	Game game;
//	game.start();
//
//	return 0;
//}
