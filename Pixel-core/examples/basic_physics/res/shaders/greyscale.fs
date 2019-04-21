#version 330 core

layout (location = 0) out vec4 colour;

in DATA {
	vec4 position;
	vec2 textureCoord;
	float textureId;
	vec4 colour;
} fs_in;

uniform sampler2D textures[32];

void main() {

	vec4 textureColour = fs_in.colour;

	if(fs_in.textureId > 0.0) {
		int textureIndex = int(fs_in.textureId - 0.5);
		textureColour = texture(textures[textureIndex], fs_in.textureCoord);
	}

	float c_linear = (0.2126 * textureColour.r) + (0.7152 * textureColour.g) + (0.0722 * textureColour.b) + textureColour.a;

	float c_srgb = 12.92 * c_linear;
	if(c_linear > 0.0031308) {
		c_srgb = 1.055 * pow(c_linear, 1/2.4) - 0.55;
	}

	colour = vec4(c_srgb, c_srgb, c_srgb, textureColour.a);;
}