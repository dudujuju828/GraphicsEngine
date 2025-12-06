#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D uDiffuseMap;
uniform int uUseTexture;

void main()
{
    vec3 base = objectColor;

    if (uUseTexture == 1) {
        base = texture(uDiffuseMap, TexCoord).rgb;
    }

    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir);

    float diff = max(dot(N, L), 0.0);

    vec3 ambient = 0.15 * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 color = (ambient + diffuse) * base;
    FragColor = vec4(color, 1.0);
}
