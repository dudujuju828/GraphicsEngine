#version 330 core
in float vHeight;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uGrassTexture;
uniform float uMinHeight;
uniform float uMaxHeight;

void main() {
    vec4 grassColor = texture(uGrassTexture, TexCoord);

    // float t = 0.0;
    // if (uMaxHeight > uMinHeight) {
    //     t = (vHeight - uMinHeight) / (uMaxHeight - uMinHeight);
    // }
    // t = clamp(t, 0.0, 1.0);
    // grassColor.rgb *= mix(0.7, 1.0, t);

    FragColor = grassColor;
}

