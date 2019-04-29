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
//	SoundManager* sounds;
//
//	Shader* shader_basic;
//	Shader* shader_greyscale;
//	Shader* shader_lighting;
//
//	Layer* background_layer;
//	Layer* hud_layer;
//	DynamicLayer* dynamic_layer;
//
//	Label* fps;
//	Sprite* fire;
//	DynamicSprite* player;
//
//	Texture* projectile;
//
//	World* world;
//
//	std::vector<DynamicSprite*> dynamic_sprites;
//
//	std::string _resource_dir;
//
//public:
//	Game() {
//		if (DEBUG)
//			_resource_dir = "examples/basic_physics/res";
//		else
//			_resource_dir = "res";
//	}
//	~Game() {
//		delete shader_basic;
//		delete shader_greyscale;
//
//		delete background_layer;
//		delete hud_layer;
//
//		delete projectile;
//	}
//
//
//	void init() override {
//		width = 32.0f;
//		height = 18.0f;
//
//		pixel_w = 1920;
//		pixel_h = pixel_w / (width / height);
//		window = createWindow("Basic Physics", pixel_w, pixel_h);
//
//		std::cout << pixel_h << std::endl;
//
//		shader_basic = new Shader(_resource_dir + "/shaders/basic.vs", _resource_dir + "/shaders/basic.fs");
//		shader_greyscale = new Shader(_resource_dir + "/shaders/greyscale.vs", _resource_dir + "/shaders/greyscale.fs");
//		shader_lighting = new Shader(_resource_dir + "/shaders/lighting.vs", _resource_dir + "/shaders/lighting.fs");
//
//		background_layer = new Layer(new BatchRenderer(), shader_lighting, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
//		hud_layer = new Layer(new BatchRenderer(), shader_basic, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
//		dynamic_layer = new DynamicLayer(new BatchRenderer(), shader_basic, mat4::orthographic(-(width / 2), (width / 2), -(height / 2), (height / 2), -1.0f, 1.0f));
//
//		sounds = new SoundManager(window->getHWND(), 44100, 15, 5, 8);
//		sounds->add(new Sound("Jump", _resource_dir + "/sounds/jump.wav"));
//
//		projectile = new Texture(_resource_dir + "/textures/red_ball.png");
//
//		Texture* bricks[] = {
//			new Texture(_resource_dir + "/textures/brick_1.png"),
//			new Texture(_resource_dir + "/textures/brick_2.png"),
//			new Texture(_resource_dir + "/textures/brick_3.png"),
//			new Texture(_resource_dir + "/textures/brick_4.png"),
//			new Texture(_resource_dir + "/textures/brick_1.png")
//		};
//
//		int map[18][32] = {
//			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
//		};
//
//
//		world = new World(vec2f(0.0f, -9.8f));
//		
//		int rows = 18;
//		int cols = 32;
//		float tile_w = width / cols;
//		float tile_h = height / rows;
//		for (int row = 0; row < rows; row++) {
//			for (int col = 0; col < cols; col++) {
//				background_layer->add(new Sprite((tile_w * col) - width / 2, (tile_h * row) - height / 2, tile_w, tile_h, bricks[rand() % 5]));
//
//				if (map[rows - (row + 1)][col] == 1) {
//					DynamicSprite* s = new DynamicSprite((tile_w * col) - width / 2, (tile_h * row) - height / 2, tile_w, tile_h, 0xBB000000);
//					s->createBody(*world);
//					s->createFixture();
//					dynamic_layer->add(s);
//					dynamic_sprites.push_back(s);
//				}
//			}
//		}
//
//
//		player = new DynamicSprite(-0.9f, 3.0f, 1.8f, 1.8f, new Texture(_resource_dir + "/textures/blue_ball.png"));
//		player->body_definition.type = DYNAMIC;
//		player->body_definition.linearDamping = 0.3f;
//		player->body_definition.fixedRotation = true;
//		player->createBody(*world);
//
//		b2CircleShape circle;
//		circle.m_radius = 0.9f;
//		player->fixture_definition.shape = &circle;
//		player->fixture_definition.density = 1.0f;
//		player->fixture_definition.friction = 0.1f;
//		player->fixture_definition.restitution = 0.0f;
//		player->createFixture(&player->fixture_definition);
//		dynamic_layer->add(player);
//		dynamic_sprites.push_back(player);
//
//
//		std::vector<Texture*> fires;
//		fires.push_back(new Texture(_resource_dir + "/textures/fire_1.png"));
//		fires.push_back(new Texture(_resource_dir + "/textures/fire_2.png"));
//		fires.push_back(new Texture(_resource_dir + "/textures/fire_3.png"));
//		fires.push_back(new Texture(_resource_dir + "/textures/fire_4.png"));
//
//		fire = new Sprite((tile_w * 13) - width / 2, (tile_h * 5) - height / 2, tile_w, tile_h, fires);
//		background_layer->add(fire);
//
//
//		FontManager::get()->setScale(window->getWidth() / 32.0f, window->getHeight() / 18.0f);
//		fps = new Label("000 fps", -15.5f, 8.1f, 0xFF00FF00);
//		hud_layer->add(fps);
//	}
//
//	void tick() override {
//		fps->text = std::to_string(getFrames()) + " fps";
//		std::cout << getUpdates() << " ups, " << getFrames() << " fps, " << (background_layer->count() + dynamic_layer->count() + hud_layer->count()) << " sprites" << std::endl;
//	}
//
//	void update() override {
//		window->getMousePos(mouse_x, mouse_y);
//		projectPixelCoords(mouse_x, mouse_y, pixel_w, pixel_h, width, height);
//
//		if (window->mousePressed(GLFW_MOUSE_BUTTON_1)) {
//			DynamicSprite* s = new DynamicSprite(mouse_x - 0.125f, mouse_y - 0.125f, 0.25f, 0.25f, 0xBBFFAA88);
//			s->body_definition.type = DYNAMIC;
//			s->body_definition.linearDamping = 0.3f;
//			s->body_definition.gravityScale = 0.3f;
//			s->createBody(*world);
//
//			b2CircleShape circle;
//			circle.m_radius = 0.125f;
//			s->fixture_definition.shape = &circle;
//			s->fixture_definition.density = 0.7f;
//			s->fixture_definition.friction = 0.0f;
//			s->fixture_definition.restitution = 0.5f;
//			s->createFixture(&s->fixture_definition);
//			background_layer->add(s);
//			dynamic_sprites.push_back(s);
//		}
//
//		if (window->keyTyped(GLFW_KEY_I)) {
//			DynamicSprite* s = new DynamicSprite(player->position.x + 0.65f, player->position.y + 1.8f, 0.5f, 0.5f, projectile);
//			s->body_definition.type = DYNAMIC;
//			s->body_definition.linearVelocity = vec2f(0.0f, 30.0f);
//			s->body_definition.linearDamping = 0.3f;
//			s->body_definition.gravityScale = 0.7f;
//			s->createBody(*world);
//
//			b2CircleShape circle;
//			circle.m_radius = 0.25f;
//			s->fixture_definition.shape = &circle;
//			s->fixture_definition.density = 1.0f;
//			s->fixture_definition.friction = 0.1f;
//			s->fixture_definition.restitution = 0.2f;
//			s->createFixture(&s->fixture_definition);
//			background_layer->add(s);
//			dynamic_sprites.push_back(s);
//		}
//
//		if (window->keyTyped(GLFW_KEY_SPACE)) {
//			sounds->play("Jump");
//			player->applyImpulse(vec2f(0.0f, 30.0f));
//		}
//
//		float speed = 15.0f;
//		if (window->keyPressed(GLFW_KEY_A))
//			player->applyForce(vec2f(-speed, 0.0f));
//		else if (window->keyPressed(GLFW_KEY_D))
//			player->applyForce(vec2f(speed, 0.0f));
//
//
//		world->step(TICK_INTERVAL / 10.0f, 16, 12);
//
//
//		for (DynamicSprite* s : dynamic_sprites) {
//			s->update();
//		}
//
//
//		fire->animate(10);
//
//
//		shader_lighting->enable();
//		shader_lighting->setUniform2f("light_pos", vec2f(player->position.x + 1.5f, player->position.y + 1.5f));
//
//
//		//shader_greyscale->enable();
//		//mat = mat4::rotation(timer->elapsedMilliseconds() / 100.0f, vec3f(0, 0, 1));
//		//shader_greyscale->setUniformMat4("ml_matrix", mat);
//	}
//
//	void render() override {
//		background_layer->render();
//		dynamic_layer->render();
//		hud_layer->render();
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
