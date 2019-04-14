#include "flatlayer.h";

namespace pixel {
	namespace graphics {

		FlatLayer::FlatLayer(Shader* shader) : Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)) {
		}

		FlatLayer::~FlatLayer() {
		}

	}
}