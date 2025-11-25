
#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 scale;

void main() {
	vec3 nPos = aPos * scale;
	gl_Position = vec4(nPos,1.0f);
}
