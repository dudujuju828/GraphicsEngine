#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aInstancePos;
layout (location = 3) in float aInstanceScale;
layout (location = 4) in float aInstanceRot;

uniform mat4 uView;
uniform mat4 uProj;
uniform float uTime;

out vec2 vTex;

void main()
{
    vec3 pos = aPos * aInstanceScale;

    float c = cos(aInstanceRot);
    float s = sin(aInstanceRot);
    mat2 rot = mat2(c, -s, s, c);
    pos.xz = rot * pos.xz;

    float bend = clamp(pos.y, 0.0, 1.0);
    pos.x += bend * 0.15 * sin(uTime * 2.0 + aInstancePos.x * 3.7 + aInstancePos.z * 1.3);

    vec4 worldPos = vec4(pos + aInstancePos, 1.0);
    gl_Position = uProj * uView * worldPos;

    vTex = aTex;
}

