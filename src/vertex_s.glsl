#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float vHeight;   // pass height to fragment shader

void main() {
    // world-space position
    vec4 worldPos = model * vec4(aPos, 1.0);

    // height is the y-component of world position
    vHeight = worldPos.y;

    gl_Position = projection * view * worldPos;
}

