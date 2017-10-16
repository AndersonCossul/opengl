#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 pmatrix; // our pmatrix
uniform mat4 smatrix; // our smatrix
uniform mat4 rmatrix; // our rmatrix

out vec3 colour;

void main() {
	colour = vertex_colour;
	vec4 eyePos = pmatrix * rmatrix * smatrix * vec4 (vertex_position, 1.0);
	gl_Position = vec4(eyePos.xy, -eyePos.z, eyePos.w);
}
