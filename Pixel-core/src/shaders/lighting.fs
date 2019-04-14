#version 330 core

layout (location = 0) out vec4 colour;

uniform vec2 light_pos;

in DATA {
	vec4 position;
	vec2 textureCoord;
	float textureId;
	vec4 colour;
} fs_in;

uniform sampler2D textures[32];

void main() {

	float size = 12.0;
	float intensity = size / (length(fs_in.position.xy - light_pos) + size);

	vec4 textureColour = fs_in.colour;

	if(fs_in.textureId > 0.0) {
		int textureIndex = int(fs_in.textureId - 0.5);
		textureColour = fs_in.colour * texture(textures[textureIndex], fs_in.textureCoord);
	}

	colour = textureColour * intensity;
}