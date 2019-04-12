#version 330 core

layout (location = 0) out vec4 colour;

uniform vec2 light_pos;

in DATA {
	vec4 position;
	vec2 textureCoord;
	vec4 colour;
} fs_in;

uniform sampler2D tex;

void main() {
	float intensity = 4.0 / length(fs_in.position.xy - light_pos);
	colour = vec4(0.1, 0.1, 0.1, 1.0) + (fs_in.colour * intensity) * 0.1;
	colour = texture2D(tex, fs_in.textureCoord) * intensity;
}