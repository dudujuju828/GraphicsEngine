#version 330 core

in vec2 vTex;
out vec4 FragColor;

uniform sampler2D uGrassTexture;

void main()
{
    vec4 tex = texture(uGrassTexture, vTex);

    if (tex.a < 0.5)
        discard;

    FragColor = tex;
}

