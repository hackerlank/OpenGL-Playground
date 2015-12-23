#version 330 core

layout(location = 0) in vec3 pos;
uniform mat4 MVP;

out vec3 frag_color;

void main() {
	vec4 v = vec4(pos, 1);
	gl_Position = MVP * v;

	frag_color[0] = mod(int(abs(pos.z) * 2), 255)/255;
	frag_color[1] = mod(int(abs(pos.y) * 256), 255)/255;
	frag_color[2] = mod(int(abs(pos.x) * 65536), 255)/255;
}
