#version 330 core

in vec3 worldPos;
in vec3 worldNormal;

out vec4 FragColor;

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 CameraPosition;

void main()
{
    vec3 worldNormalN = normalize(worldNormal);
    vec3 worldLightDir = normalize(LightDirection);
    vec3 worldViewDir = normalize(CameraPosition - worldPos);

    vec3 ambient = LightColor * 0.1;
    vec3 diffuse = LightColor * max(dot(worldNormalN, worldLightDir), 0.0);
    vec3 specular = LightColor * pow(max(dot(worldNormalN, normalize(worldViewDir + worldLightDir)), 0.0), 20.0);

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
