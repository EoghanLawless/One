#pragma once

#include <string>

#include "FreeImage.h"
#include "GL/glew.h"

#include "utils/imageload.h"

namespace pixel {
	namespace graphics {

		class Texture {
		private:
			std::string _path;
			GLuint _textureId;
			GLsizei _width, _height;

			GLuint load();

		public:
			Texture(const std::string& path);
			~Texture();

			void bind() const;
			void unbind() const;

			inline const unsigned int getId() const { return _textureId; }
			inline const unsigned int getWidth() const { return _width; }
			inline const unsigned int getHeight() const { return _height; }
		};

	}
}