#pragma once

#include "layer.h"

#include "graphics/renderers/batchrenderer.h"

namespace pixel {
	namespace graphics {

		class FlatLayer : public Layer {
		public:
			FlatLayer(Shader* shader);
			virtual ~FlatLayer();
		};

	}
}