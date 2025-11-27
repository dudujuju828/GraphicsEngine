#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightDir;   
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    vec3 N = normalize(Normal);
    vec3 L = normalize(-lightDir); 

    float diff = max(dot(N, L), 0.0);

    vec3 ambient = 0.15 * lightColor;
    vec3 diffuse = diff * lightColor;

    vec3 color = (ambient + diffuse) * objectColor;
    FragColor = vec4(color, 1.0);
}

