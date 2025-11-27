#version 330 core
in vec3 vNormal;
in vec3 vWorldPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D uGrassTexture;
uniform vec3 uLightDir;
uniform vec3 uLightColor;
uniform vec3 uAmbientColor;
uniform float uMinHeight;
uniform float uMaxHeight;

void main() {
    vec3 albedo = texture(uGrassTexture, TexCoord).rgb;
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-uLightDir);
    float NdotL = max(dot(N, L), 0.0);
    vec3 diffuse = NdotL * uLightColor * albedo;
    vec3 ambient = uAmbientColor * albedo;
    vec3 color = diffuse + ambient;
    FragColor = vec4(color, 1.0);
}

