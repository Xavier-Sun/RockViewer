#version 330 core

in vec3 worldPos;
in vec3 worldNormal;
in vec2 uv;

out vec4 gl_FragColor;

uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 CameraPosition;

uniform vec3 Color;
uniform sampler2D DiffuseMap;

void main()
{
    vec3 worldNormalN = normalize(worldNormal);
    vec3 worldLightDir = normalize(LightDirection);
    vec3 worldViewDir = normalize(CameraPosition - worldPos);
    vec3 albedo = vec3(texture(DiffuseMap, uv)) * Color;

    vec3 ambient = LightColor * albedo * 0.1;
    vec3 diffuse = LightColor * albedo * max(dot(worldNormalN, worldLightDir), 0.0);
    vec3 specular = LightColor * pow(max(dot(worldNormalN, normalize(worldViewDir + worldLightDir)), 0.0), 20.0);

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
