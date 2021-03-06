#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in float textureId;
layout (location = 3) in vec4 colour;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA {
	vec4 position;
	vec2 textureCoord;
	float textureId;
	vec4 colour;
} vs_out;

void main() {
	gl_Position = pr_matrix * ml_matrix * vw_matrix * position;
	vs_out.position = ml_matrix * position;
	vs_out.textureCoord = textureCoord;
	vs_out.textureId = textureId;
	vs_out.colour = colour;
}