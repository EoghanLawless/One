#pragma once

#include "layer.h"
#include "../batchrenderer2d.h"

namespace pixel {
	namespace graphics {
		class FlatLayer : public Layer {
		public:
			FlatLayer(Shader* shader);
			virtual ~FlatLayer();
		};
	}
}