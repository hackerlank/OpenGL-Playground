#version 330 core

layout(location = 0) in vec3 pos;
uniform mat4 MVP;
uniform bool line;

out vec3 frag_color;

void main() {
	vec4 v = vec4(pos, 1);
	gl_Position = MVP * v;

	if (line) {
		frag_color = vec3(0, 0, 0);
	}
	else {
		frag_color[0] = (abs(pos.z + 16) / 32.0);
		frag_color[1] = (abs(pos.y + 16) / 32.0);
		frag_color[2] = (abs(pos.x + 16) / 32.0);
	}
}
