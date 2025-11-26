#version 330 core
in float vHeight;
out vec4 FragColor;

uniform float uMinHeight;
uniform float uMaxHeight;

void main() {
    float t = 0.0;

    // avoid division by zero in case uMinHeight == uMaxHeight
    if (uMaxHeight > uMinHeight) {
        t = (vHeight - uMinHeight) / (uMaxHeight - uMinHeight);
    }

    t = clamp(t, 0.0, 1.0);

    // black (low) -> white (high)
    FragColor = vec4(vec3(t), 1.0);
}

