//#include "pixel.h"
//
//#include "res/layers/dynamiclayer.h"
//
//#define DEBUG 1
//
//using namespace pixel;
//using namespace graphics;
//using namespace physics;
//using namespace audio;
//using namespace maths;
//
//class Game : public Pixel {
//
//private:
//	float width, height;
//	int pixel_w, pixel_h;
//	double mouse_x, mouse_y;
//
//	Window* window;
//
//	Shader* shader;
//
//	DynamicLayer* layer;
//
//	DynamicSprite* force;
//
//	World* world;
//
//	std::vector<DynamicSprite*> sprites;
//
//	std::string _resource_dir;
//
//public:
//	Game() {
//		if (DEBUG)
//			_resource_dir = "examples/pixel_art/res";
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
//		pixel_w = 1200;
//		pixel_h = pixel_w / (width / height);
//		window = createWindow("Basic Physics", pixel_w, pixel_h);
//		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//
//		shader = new Shader(_resource_dir + "/shaders/basic.vs", _resource_dir + "/shaders/basic.fs");
//		layer = new DynamicLayer(new BatchRenderer(), shader, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
//
//		world = new World(vec2f(0.0f, -9.8f));
//		
//		int rows = 18;
//		int cols = 32;
//		float tile_w = width / cols;
//		float tile_h = height / rows;
//		for (int row = 0; row < rows; row++) {
//			for (int col = 0; col < cols; col++) {
//				if (row == 0 || row == rows - 1 || col == 0 || col == cols - 1) {
//					DynamicSprite* s = new DynamicSprite((tile_w * col) - width / 2, (tile_h * row) - height / 2, tile_w, tile_h, 0x00FFFFFF);
//					s->createBody(*world);
//					s->createFixture();
//					layer->add(s);
//					sprites.push_back(s);
//				}
//			}
//		}
//
//
//		force = new DynamicSprite(-0.9f, 3.0f, 1.8f, 1.8f, 0x00FFFFFF);
//		force->body_definition.type = DYNAMIC;
//		force->body_definition.linearDamping = 5.0f;
//		force->body_definition.gravityScale = 0.0f;
//		force->body_definition.fixedRotation = true;
//		force->createBody(*world);
//
//		force->fixture_definition.density = 1.0f;
//		force->fixture_definition.friction = 0.0f;
//		force->fixture_definition.restitution = 0.0f;
//		force->createFixture(&force->fixture_definition);
//		layer->add(force);
//		sprites.push_back(force);
//	}
//
//	void tick() override {
//		std::cout << getUpdates() << " ups, " << getFrames() << " fps, " << layer->count() << " sprites" << std::endl;
//	}
//
//	void update() override {
//		window->getMousePos(mouse_x, mouse_y);
//		projectPixelCoords(mouse_x, mouse_y, pixel_w, pixel_h, width, height);
//
//		force->applyForce(vec2f(((mouse_x - 0.9f) - force->position.x) * 20, ((mouse_y - 0.9f) - force->position.y) * 20));
//
//		if (window->mousePressed(GLFW_MOUSE_BUTTON_1)) {
//			DynamicSprite* s = new DynamicSprite(mouse_x - 0.125f, mouse_y - 0.125f, 0.25f, 0.25f, 0xBBFFAA88);
//			s->body_definition.type = DYNAMIC;
//			s->body_definition.linearDamping = 0.3f;
//			s->body_definition.gravityScale = -0.3f;
//			s->createBody(*world);
//
//			b2CircleShape circle;
//			circle.m_radius = 0.40f;
//			s->fixture_definition.shape = &circle;
//			s->fixture_definition.density = 0.7f;
//			s->fixture_definition.friction = 0.0f;
//			s->fixture_definition.restitution = 0.5f;
//			s->createFixture(&s->fixture_definition);
//			layer->add(s);
//			sprites.push_back(s);
//		}
//
//		if (window->mousePressed(GLFW_MOUSE_BUTTON_2)) {
//			for (DynamicSprite* s : sprites) {
//				s->applyForce(vec2f(((mouse_x - 0.125f) + force->position.x) * 20, ((mouse_y - 0.125f) + force->position.y) * 20));
//			}
//		}
//
//		world->step(TICK_INTERVAL / 10.0f, 16, 12);
//
//		for (DynamicSprite* s : sprites) {
//			s->update();
//		}
//
//	}
//
//	void render() override {
//		layer->render();
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
